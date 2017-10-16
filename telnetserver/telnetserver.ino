
//PLEASE NOTE:
//require ESP8266 >= 2.4.0 https://github.com/esp8266/Arduino/releases/tag/2.4.0-rc1
//for the use of "setRxBufferSize" function

#include <Arduino.h>
#include <ESP8266WiFi.h> 
#include <Hash.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;
const char* update_path = "/firmware";
const char* update_username = "admin";
const char* update_password = "admin";


#define MAX_SRV_CLIENTS 1
WiFiServer server(23);
WiFiClient serverClient;

int RESET_PIN = 0; // = GPIO0 on nodeMCU
WiFiManager wifiManager;

void setup()
{
	Serial.setRxBufferSize(1024); //require ESP8266 >= 2.4.0 https://github.com/esp8266/Arduino/releases/tag/2.4.0-rc1
	Serial.begin(115200);

    delay(5000); //BOOT WAIT
    pinMode(RESET_PIN, INPUT_PULLUP);
    wifiManager.autoConnect("ESP8266");

	server.begin();
	server.setNoDelay(true);

    httpUpdater.setup(&httpServer, update_path, update_username, update_password);
    httpServer.begin();

    WiFi.setSleepMode(WIFI_NONE_SLEEP); // disable WiFi sleep for more performance
}


void loop()
{
	if (server.hasClient())
		AcceptConnection();
	else if (serverClient && serverClient.connected())
		ManageConnected();
	
	httpServer.handleClient();
}

void AcceptConnection()
{
	if (serverClient && serverClient.connected()) 
		serverClient.stop();

	serverClient = server.available();
	serverClient.write("ESP8266 Connected!\n");
}

void ManageConnected()
{
    size_t rxlen = serverClient.available();
    if (rxlen > 0)
	{
		uint8_t sbuf[rxlen];
		serverClient.readBytes(sbuf, rxlen);
        Serial.write(sbuf, rxlen);
	}
	
    size_t txlen = Serial.available();
    if (txlen > 0)
	{
		uint8_t sbuf[txlen];
		Serial.readBytes(sbuf, txlen);
        serverClient.write(sbuf, txlen);
	}
}


