// Ublox GPS Neo8M MQTT Example  rb July 8,2025
// This example expects that the NEO8M GPS has been configured using the Neo8mConfig example
// the Neo8m should be set to 115k baud with UBX output 10 times per second
// 


#include <UbxGpsNavPvt8.h>
#include <UbxGpsNavSat.h>
#include <PicoMQTT.h>
#include <Wire.h>
#include <WiFi.h>
#include <time.h>

// uncomment to create a WiFi access point in which you can connect your phone's WiFi
// otherwise, we'll connect to an existing WiFi access point
//#define WIFI_ACCESS_POINT

#ifndef WIFI_SSID
#define WIFI_SSID "TheMudgkins"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "1mudgkins2"
#endif

// pins for NEO GPS Module
static const int RXPin = 4, TXPin = 5;

#define COMPUTER_BAUDRATE 115200
#define GPS_BAUDRATE 115200

#define DATETIME_FORMAT "%04d.%02d.%02d %02d:%02d:%02d"
#define DATETIME_LENGTH 20

UbxGpsNavPvt8 pvt;
UbxGpsNavSat sat;
PicoMQTT::Server mqtt;
unsigned long lastTime;
const char *fixType[] = { "0: no fix", "1: dead reckoning only", "2: 2D-fix", "3: 3D-fix", "4: GNSS + dead reckoning combined", "5: time only fix", "Unknown" };
char datetime[DATETIME_LENGTH];

void setup() {
  Serial.begin(COMPUTER_BAUDRATE);
  Serial1.begin(GPS_BAUDRATE, SERIAL_8N1, RXPin, TXPin);    // 2nd serial port for Neo8m module
  Wire.begin();   // start i2c bus

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
  mqtt.begin();

  lastTime = millis();
}

void loop() {
  unsigned char b;

  mqtt.loop();
  // put your main code here, to run repeatedly:
  while(Serial1.available())
  {
    mqtt.loop();
    b = Serial1.read();
    if (pvt.ready(b))
    {
        if (millis() - lastTime >= 1000)
        {
          // update the MQTT values for the phone app once/second
          lastTime = millis();
          snprintf(datetime, DATETIME_LENGTH, DATETIME_FORMAT, pvt.NavPvt8.year, pvt.NavPvt8.month, pvt.NavPvt8.day, pvt.NavPvt8.hour, pvt.NavPvt8.min, pvt.NavPvt8.sec);
          mqtt.publish(String("gps/date"), datetime);
          Serial.print("GPS Date: ");
          Serial.println(datetime);
          double lon = pvt.NavPvt8.lon / 10000000.0;
          mqtt.publish(String("gps/longitude"), String(lon, 5));
          double lat = pvt.NavPvt8.lat / 10000000.0;
          mqtt.publish(String("gps/latitude"), String(lat, 5));
          //Serial.print(pvt.NavPvt8.hMSL / 1000.0, 3);
          // speed over ground is in mm/s but we want knots
          double dspeed = pvt.NavPvt8.gSpeed * 0.00194384;
          mqtt.publish(String("gps/sog"), String(dspeed, 1));
          double headMot = pvt.NavPvt8.headMot / 100000.0;
          mqtt.publish(String("gps/cog"), String(headMot, 1));
          int fixndx = pvt.NavPvt8.fixType;
          if (fixndx >= sizeof(fixType))
            fixndx = sizeof(fixType) - 1;
          mqtt.publish(String("gps/fix"), String(fixType[fixndx]));
          double pdop = pvt.NavPvt8.pDOP / 100.0;
          mqtt.publish(String("gps/pdop"), String(pdop, 2));
          mqtt.publish(String("gps/numsv"), String(pvt.NavPvt8.numSV));
        }
    }
    if (sat.ready(b))
    {
        Serial.print(sat.NavSat.iTOW);
        Serial.print(',');
        Serial.println(sat.NavSat.numSvs);
        for(int i = 0; i < sat.NavSat.numSvs; i++)
        {
          Serial.printf(" gid %d svId %d cno %d azim %d flags %.4X", sat.NavSat.svs[i].gnssId, sat.NavSat.svs[i].svId, sat.NavSat.svs[i].cno, sat.NavSat.svs[i].azim, sat.NavSat.svs[i].flags);
          if ((i % 3) == 0)
            Serial.println();
        }
        Serial.println();
    }
  }

}
