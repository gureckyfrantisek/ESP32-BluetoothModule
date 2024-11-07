# ESP32-BluetoothModule

 Code for ESP32 to work as a Bluetooth module for uBlox ZED-F9P

The module: https://www.laskakit.cz/iot-esp-32s-2-4ghz-dual-mode-wifi-bluetooth-rev-1--cp2102/

Developed in Arduino IDE

Setup:

1) In the IDE, go to File -> Preferences -> Aditional board manager URLs -> paste this: https://dl.espressif.com/dl/package_esp32_index.json

2) Got to Board manager -> Search: "esp32" -> Download: "esp32 by Espressif Systems"

3) Download the "CP210x VCP Windows" from https://www.silabs.com/developer-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads

And you're all set!

Now when compiling the code to the board, hold down the "Boot" button for the ESP to enter "Download mode"