# ESP Wake on Lan
ESP Wake on Lan is a simple web interface on an ESP32/ESP8266 to send Wake on Lan Magic Packets.

The MAC address of the device to wake up can be selected from a list of specified devices, or manually entered.  
The default list of devices to wake up is an external file that is compiled into the project.  
Manually entered MAC addresses are added to the dropdown, however they are lost when the ESP restarts.  
It is also possible to specify the target broadcast IP address to send the packet to.  
This is done by selecting one from a dropdown which contains a default option specified in the source code,  
the last selected IP(if that is a different one), and 255.255.255.255, or manually specifying one.  
To manually specify a MAC or IP address simply select the "Other" option and enter it into the text input that appears.  
This is how the web interface looks:  
![ESP Wake on Lan Web Interface](https://raw.githubusercontent.com/ToMe25/ESP-WakeOnLan/master/images/web_interface.png)

## Requirements
 1. A system with a working [platform io](https://platformio.org/) installation.
 2. A ESP32 or ESP8266 board, tested only with the ESP32-DevKitC and ESP8266-DevKitC but should work with others.

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
 * Run `pio run -t upload -e esp32dev` to compile this project and flash it to your ESP32 or `pio run -t upload -e esp_wroom_02` for ESP8266.
 * To later update it over WiFi run `pio ron -t upload -e esp32dev_ota` for ESP32 or `pio run -t upload -e esp_wroom_02_ota` for ESP8266.
