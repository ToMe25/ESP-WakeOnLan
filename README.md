# ESP Wake on Lan
ESP Wake on Lan is a simple web interface on an ESP32 to send Wake on Lan Magic Packets to one out a list of specified devices.
The device to wake up can be selected from a list in an external file that is compiled into the project.
It is also possible to specify the target (broadcast) address to send the packet to.
This is how the web interface looks:  
![ESP Wake on Lan Web Interface](https://raw.githubusercontent.com/ToMe25/ESP-WakeOnLan/master/images/web-interface.png)

## Requirements
 1. A system with a working [platform io](https://platformio.org/) installation.
 2. A ESP32 board, tested only with the ESP32-devkitc, but should work with others.

## Installation
 * Create a file called `wifissid.txt` in the root folder of this project, containing your wifi ssid.
 * Create a file called `wifipass.txt` in the project root containing the wpa passphrase of your wifi.
 * Create a file called `otapass.txt` containing the password for [Arduino Over The Air](https://www.arduino.cc/reference/en/libraries/arduinoota/) firmware updates.
 * Make sure these files don't end with an empty line.
 * Put the MAC addresses of the devices to wake up to a file called `devices.txt` in the project root.
 * Put one MAC address in each line.
 * An example can be seen in `devices.example`.
 * Empty lines in this file are ignored.
 * Attach your ESP to your PC.
 * Run `pio run -t upload -e esp32dev` to compile this project and flash it to your ESP.
 * To later update it over WiFi run `pio ron -t upload -e ota`.
