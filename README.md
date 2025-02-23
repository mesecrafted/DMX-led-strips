# DMX512 controlled LED strips

This project was thought up to make custom led strip fixtures controllable with DMX512. I used inexpensive parts to build a decent quality addressable LED fixture. 

## The hardware I used

Obviously you don't need to used these exact parts, but this is what I used

- [ESP32](https://a.co/d/0fAkLVg)
- [Level shifter](https://a.co/d/aY0sSCC)
- [Addressable LEDs](https://a.co/d/81Wl8qD)
- [Diffuser Channels](https://a.co/d/hbj3Ljh)
- [Power supply](https://a.co/d/jcvAAeS)
- [XLR connectors]()
- [Power connectors](https://a.co/d/3cF8bFB)
- [Enclosure](https://a.co/d/8EDo1iI)

## Downloading and compiling the code 

I used the VS Code IDE with the PlatformIO extention. This repo includes my `platformio.ini` file, so PlatformIO should automatically download the libraries you need. 

Before compiling the code make sure to check the constants set near the top of `src/main.cpp`. 