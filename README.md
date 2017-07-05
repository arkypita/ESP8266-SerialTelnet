
`1.`  Download and install Arduino IDE 1.6.x from https://www.arduino.cc/en/Main/Software

`2.`  Install ESP8266 Arduino support https://github.com/esp8266/Arduino#installing-with-boards-manager

`3.`  Install the following libraries:

`3.1:`  WifiManager: https://github.com/tzapu/WiFiManager#install-through-library-manager

`4.` Configure your ESP8266 for sketch upload (GIPOs pulled up and down accordingly, USB to serial connected, reset and ready for upload)

`5.`  Upload the Sketch contained in `telnetserver` folder

`6.`  Connect the TX of the ESP8266 to RX of grbl arduino board, and RX of the ESP8266 to TX. Power up the ESP and engraver

`7.`  Connect to the ESP8266 WiFi network and go to "http:\\192.168.4.1". Configure connection to your AP, set password, then switch back to your local wifi (Animation below shows the details)

`8.` In LaserGRBL open menu "grbl-settings" and configure for `Telnet` protocol and connect to the IP of the ESP (usually 192.168.x.y:23 where "x" is your subnet and "y" assigned by DHCP)

NOTE: I will add an IP scanner soon. For now, check on your DHCP server which IP was dished out)

![Setting Up Wifi](wifibridge.gif)

