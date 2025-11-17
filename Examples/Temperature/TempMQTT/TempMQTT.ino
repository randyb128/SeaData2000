// TempMQTT - example for DSTemp library for SeaData 2000 board
// rb Nov 13/2025
// reads up to 6 Dallas DS18B20 devices and returns the current temperature in an array
// has two dependencies - DS2482 library by Joe Young
//                      - picomqtt library
//
// this will publish the temperature to seadata/temp/chanX 
// where X = 1-6 for up to 6 channels of temperature devices
//
#include <PicoMQTT.h>
#include <Wire.h>
#include <WiFi.h>

// uncomment to create a WiFi access point in which you can connect your phone's WiFi
// otherwise, we'll connect to an existing WiFi access point
//#define WIFI_ACCESS_POINT

#include "DSTemp.h"   // SeaData Dallas One Wire temperature library

#define I2Cadr 0x18   //base address of DS2482

DSTemp dsTemp(I2Cadr);  // create our temperature object

#ifndef WIFI_SSID
#define WIFI_SSID "YourSSID"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "YourPassword"
#endif

PicoMQTT::Server mqtt;

unsigned long last_publish_time = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial){ /* wait */ }
  Wire.begin();   // start i2c bus
  dsTemp.begin();
  if (dsTemp.lastError != DS_OK)
  {
    Serial.print("DSTemp error ");
    Serial.println(dsTemp.lastError);
  }

#ifdef WIFI_ACCESS_POINT
  // Create WiFi Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
  delay(2000);
  IPAddress ip = WiFi.softAPIP();
  Serial.print("WiFi server IP: ");
  Serial.println(ip);
#else
  // Connect to existing WiFi Access Point
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) { delay(1000); }
  delay(2000);
  Serial.printf("WiFi connected, IP: %s\n", WiFi.localIP().toString().c_str());
#endif

  // Subscribe to a topic and attach a callback
  mqtt.subscribe("#", [](const char * topic, const char * payload) {
      // payload might be binary, but PicoMQTT guarantees that it's zero-terminated
      Serial.printf("Received message in topic '%s': %s\n", topic, payload);
  });

  mqtt.begin();
  last_publish_time = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  dsTemp.loop();
  mqtt.loop();

  // Publish a temperature value every 3 seconds.
  if (millis() - last_publish_time >= 3000) 
  {
    for(int ndx = 0; ndx < dsTemp.numTemp; ndx++)
    {
      // We're publishing to a topic, which we're subscribed too, but these message will *not* be delivered locally.
      String topic = "temp/chan" + String(ndx);
      String message = String(dsTemp.ftemps[ndx]);
      Serial.printf("Publishing message in topic '%s': %s\n", topic.c_str(), message.c_str());
      mqtt.publish(topic, message);
    }
    last_publish_time = millis();
  }
}
