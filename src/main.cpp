/*

  DMX receive and translate to addressable RGB

  Large portions of the DMX related code comes from the esp_dmx library example code
  Created 9 September 2021 by Mitch Weisbrod under the MIT license
  https://github.com/someweisguy/esp_dmx

*/
#include <Arduino.h>
#include <esp_dmx.h>
#include <Adafruit_NeoPixel.h>
//neopixel values
#define LED_PIN1 13 // The pin used for strip 1
#define LED_PIN2 12 // The pin used for strip 2
#define LED_COUNT 142 // Number of LED chips in a strip
#define GROUP_SIZE 9 // LED_COUNT / # of groups
#define START_ADDR 440 //DMX Start Address (360)

//Define tasks for multithreading
TaskHandle_t DMX_loop;
TaskHandle_t LED_loop;

//define the thread functions ahead of time for compiling
void DMX_Loop_Func(void * pvParameters);
void LED_Loop_Func(void * pvParameters);

//define neopixel strip objects
Adafruit_NeoPixel strip1(LED_COUNT, LED_PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(LED_COUNT, LED_PIN2, NEO_GRB + NEO_KHZ800);

/* First, lets define the hardware pins that we are using with our ESP32. We
  need to define which pin is transmitting data and which pin is receiving data.
  DMX circuits also often need to be told when we are transmitting and when we
  are receiving data. We can do this by defining an enable pin. */
int transmitPin = 17;
int receivePin = 16;
int enablePin = 21;
/* Make sure to double-check that these pins are compatible with your ESP32!
  Some ESP32s, such as the ESP32-WROVER series, do not allow you to read or
  write data on pins 16 or 17, so it's always good to read the manuals. */

/* Next, lets decide which DMX port to use. The ESP32 has either 2 or 3 ports.
  Port 0 is typically used to transmit serial data back to your Serial Monitor,
  so we shouldn't use that port. Lets use port 1! */
dmx_port_t dmxPort = 1;

/* Now we want somewhere to store our DMX data. Since a single packet of DMX
  data can be up to 513 bytes long, we want our array to be at least that long.
  This library knows that the max DMX packet size is 513, so we can fill in the
  array size with `DMX_PACKET_SIZE`. */
byte data[DMX_PACKET_SIZE];

/* The last two variables will allow us to know if DMX has been connected and
  also to update our packet and print to the Serial Monitor at regular
  intervals. */
bool dmxIsConnected = false;
//unsigned long lastUpdate = millis();

void setup() {
  /* Start the serial connection back to the computer so that we can log
    messages to the Serial Monitor. Lets set the baud rate to 115200. */
  //115200 caused errors in the serial monitor. I didn't bother to troubleshoot why; I have no reason to used such a high rate. 
  Serial.begin(9600);

  /* begin the nepixel strips */
  strip1.begin();           // Initialize NeoPixel object
  strip1.setBrightness(255); // Brightness 0-255
  strip2.begin();
  strip2.setBrightness(255);


  /* Now we will install the DMX driver! We'll tell it which DMX port to use,
    what device configuration to use, and what DMX personalities it should have.
    If you aren't sure which configuration to use, you can use the macros
    `DMX_CONFIG_DEFAULT` to set the configuration to its default settings.
    This device is being setup as a DMX responder so it is likely that it should
    respond to DMX commands. It will need at least one DMX personality. Since
    this is an example, we will use a default personality which only uses 1 DMX
    slot in its footprint. */
  dmx_config_t config = DMX_CONFIG_DEFAULT;
  dmx_personality_t personalities[] = {
    {1, "Default Personality"}
  };
  int personality_count = 1;
  dmx_driver_install(dmxPort, &config, personalities, personality_count);

  /* Now set the DMX hardware pins to the pins that we want to use and setup
    will be complete! */
  dmx_set_pin(dmxPort, transmitPin, receivePin, enablePin);


  xTaskCreatePinnedToCore(
    DMX_Loop_Func,   /* Task function. */
    "DMX_loop",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &DMX_loop,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */
  delay(500);

  xTaskCreatePinnedToCore(
    LED_Loop_Func,   /* Task function. */
    "LED_loop",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &LED_loop,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */
  delay(500);
}

void DMX_Loop_Func(void * pvParameters) {
  for( ;; ){ //without a loop the esp crashes
    /* We need a place to store information about the DMX packets we receive. We
      will use a dmx_packet_t to store that packet information.  */
    dmx_packet_t packet;

    /* And now we wait! The DMX standard defines the amount of time until DMX
      officially times out. That amount of time is converted into ESP32 clock
      ticks using the constant `DMX_TIMEOUT_TICK`. If it takes longer than that
      amount of time to receive data, this if statement will evaluate to false. */
    if (dmx_receive(dmxPort, &packet, DMX_TIMEOUT_TICK)) {
      /* If this code gets called, it means we've received DMX data! */

      /* Get the current time since boot in milliseconds so that we can find out
        how long it has been since we last updated data and printed to the Serial
        Monitor. 
      unsigned long now = millis();*/

      /* We should check to make sure that there weren't any DMX errors. */
      if (!packet.err) {
        /* If this is the first DMX data we've received, lets log it! */
        if (!dmxIsConnected) {
          Serial.println("DMX is connected!");
          dmxIsConnected = true;
        }

        /* Don't forget we need to actually read the DMX data into our buffer so
          that we can print it out. */
        dmx_read(dmxPort, data, packet.size);

        //removed LED code from here and put it in the other thread        

        /* // The following code is to send periodical messages to the serial monitor (I broke it in the process of disabling it)
        if (now - lastUpdate > 10) {
          Serial.printf("Start code is 0x%02X and slot 1 is 0x%02X\n", data[0],
                        data[1]);
          lastUpdate = now;
        }
        */

      } else {
        /* Oops! A DMX error occurred! Don't worry, this can happen when you first
          connect or disconnect your DMX devices. If you are consistently getting
          DMX errors, then something may have gone wrong with your code or
          something is seriously wrong with your DMX transmitter. */
        Serial.println("A DMX error occurred.");
      }
    } else if (dmxIsConnected) {
      /* If DMX times out after having been connected, it likely means that the
        DMX cable was unplugged. When that happens in this example sketch, we'll
        uninstall the DMX driver. No we won't not sure if I should do this differently, but I got rid of this*/
      Serial.println("DMX was disconnected.");
      dmxIsConnected = false;
      for ( int i = 0; i <= 512; i++) {
        data[i] = 0; 
      }
      //dmx_driver_delete(dmxPort); // I want the driver to stay just to be safe
      /* Stop the program. */
      //while (true) yield();
    }
  }
}

void LED_Loop_Func(void * pvParameters) {
  for(;;) {
    strip1.clear(); // Set all pixel colors to 'off'
    strip2.clear(); // Set all pixel colors to 'off'

    // The first NeoPixel in a strand is #0, second is 1, all the way up
    // to the count of pixels minus one.
    for(int i=0; i<LED_COUNT; i++) {
      /* You would change the multiplier if each led had more channels. ex: RGBW led would use "* 4" instead of "* 3" */
      int group_offset = START_ADDR + (i/GROUP_SIZE) * 3;
      strip1.setPixelColor( i, data[group_offset], data[group_offset + 1], data[group_offset + 2] );
      strip2.setPixelColor( i, data[group_offset], data[group_offset + 1], data[group_offset + 2] );
    }
        
    strip1.show();
    strip2.show();
  }
}

void loop() {
}
