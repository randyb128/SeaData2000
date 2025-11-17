#include <UbxGpsNavPvt8.h>
#include <UbxGpsNavSat.h>
#include <BLEParams.h>

// pins for NEO GPS Module
static const int RXPin = 4, TXPin = 5;

#define COMPUTER_BAUDRATE 115200
#define GPS_BAUDRATE 115200

#define DATETIME_FORMAT "%04d.%02d.%02d %02d:%02d:%02d"
#define DATETIME_LENGTH 20

UbxGpsNavPvt8 pvt;
UbxGpsNavSat sat;

BLEParams pserv;
unsigned long lastTime;

char datetime[DATETIME_LENGTH];

enum MonitorNdx { LON, LAT, SPEED, SATS };

void setup() {
  Serial.begin(COMPUTER_BAUDRATE);
  Serial1.begin(GPS_BAUDRATE, SERIAL_8N1, RXPin, TXPin);
  pserv.AddToMonitor(LON, "Lon", 0.0, 7);
  pserv.AddToMonitor(LAT, "Lat", 0.0, 7);
  pserv.AddToMonitor(SPEED, INT_V, "Speed", 0);
  pserv.AddToMonitor(SATS, INT_V, "Sats", 0);
  pserv.begin(String("SeaData"));
  lastTime = millis();
}

void loop() {
  unsigned char b;
  // put your main code here, to run repeatedly:
  while(Serial1.available())
  {
    b = Serial1.read();
    if (pvt.ready(b))
    {
        if (millis() - lastTime >= 1000)
        {
        // update the phone app values once/second
          lastTime = millis();
          snprintf(datetime, DATETIME_LENGTH, DATETIME_FORMAT, pvt.NavPvt8.year, pvt.NavPvt8.month, pvt.NavPvt8.day, pvt.NavPvt8.hour, pvt.NavPvt8.min, pvt.NavPvt8.sec);

          Serial.print(datetime);
          Serial.print(',');
          double lon = pvt.NavPvt8.lon / 10000000.0;
          Serial.print(lon, 7);
          Serial.print(',');
          double lat = pvt.NavPvt8.lat / 10000000.0;
          Serial.print(lat, 7);
          Serial.print(',');
          Serial.print(pvt.NavPvt8.height / 1000.0, 3);
          Serial.print(',');
          Serial.print(pvt.NavPvt8.hMSL / 1000.0, 3);
          Serial.print(',');
          double dspeed = pvt.NavPvt8.gSpeed * 0.0036;
          Serial.print(dspeed, 5);
          Serial.print(',');
          Serial.print(pvt.NavPvt8.headMot / 100000.0, 5);
          Serial.print(',');
          Serial.print(pvt.NavPvt8.fixType);
          Serial.print(',');
          Serial.print(pvt.NavPvt8.pDOP / 100.0, 2);
          Serial.print(',');
          Serial.println(pvt.NavPvt8.numSV);
          pserv.SetMonitorValue(LON, lon);
          pserv.SetMonitorValue(LAT, lat);
          pserv.SetMonitorValue(SPEED, (int)dspeed);
          pserv.SetMonitorValue(SATS, (int)pvt.NavPvt8.numSV);
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
    pserv.loop();   // keep web server running
  }

}
