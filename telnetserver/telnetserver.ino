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
WiFiClient serverClients[MAX_SRV_CLIENTS];

int RESET_PIN = 0; // = GPIO0 on nodeMCU
WiFiManager wifiManager;

void setup()
{
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
    httpServer.handleClient();
    manage();
}

void manage() {
  uint8_t i;
  //check if there are any new clients
  if (server.hasClient()){
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      //find free/disconnected spot
      if (!serverClients[i] || !serverClients[i].connected()){
        if(serverClients[i]) serverClients[i].stop();
        serverClients[i] = server.available();
        serverClients[i].write("Connected\n");
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
  //check clients for data
  for(i = 0; i < MAX_SRV_CLIENTS; i++){
    if (serverClients[i] && serverClients[i].connected()){
      if(serverClients[i].available()){
        //get data from the telnet client and push it to the UART
        while(serverClients[i].available()) Serial.write(serverClients[i].read());
      }
    }
  }
  //check UART for data
  if(Serial.available()){
    size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
    //push UART data to all connected telnet clients
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      if (serverClients[i] && serverClients[i].connected()){
        serverClients[i].write(sbuf, len);
        delay(1);
      }
    }
  }
}
