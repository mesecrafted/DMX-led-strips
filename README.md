# DMX512 controlled LED strips

This project makes custom addressable LED strip fixtures controllable with DMX512. We used inexpensive parts to build a decent quality addressable LED fixture. 

## The hardware we used  

- [ESP32](https://a.co/d/0fAkLVg)
- [Level shifter](https://a.co/d/aY0sSCC)
- [Addressable LEDs](https://a.co/d/81Wl8qD)
- [Diffuser Channels](https://a.co/d/hbj3Ljh)
- [Power supply](https://a.co/d/jcvAAeS)
- [XLR connectors](https://a.co/d/0RO5Dlo)
- [Power connectors](https://a.co/d/3cF8bFB)
- [JST connectors](https://a.co/d/9SE8yq0)
- [Enclosure](https://a.co/d/8EDo1iI)
- Mic stands for mounting lights  
- 3D printed parts (files included) to mount fixtures and controllers  

## Downloading and compiling the code 

The software can be compiled and installed on a ESP32 with the PlatformIO vscode extension. This repo includes a `platformio.ini` file, so PlatformIO should automatically download the libraries you need. 

Before compiling the code make sure to check the constants set near the top of `src/main.cpp`. 