// TempSimple - example for DSTemp library for SeaData 2000 board
// rb Nov 13/2025
// reads up to 6 Dallas DS18B20 devices and returns the current temperature in an array
// has one dependency - DS2482 library by Joe Young
//
#include <Wire.h>
#include "DSTemp.h"   // SeaData Dallas One Wire temperature library

#define I2Cadr 0x18   //base address of DS2482

DSTemp dsTemp(I2Cadr);  // create our temperature object
unsigned long dsTime;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial){ /* wait */ }
  Wire.begin();   // start i2c bus
  dsTemp.fahrenheit = true;
  dsTemp.begin();
  if (dsTemp.lastError != DS_OK)
  {
    Serial.print("DSTemp error ");
    Serial.println(dsTemp.lastError);
  }
  dsTime = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  dsTemp.loop();
  if (millis() - dsTime >= 1000UL)
  {
    for(int i = 0; i < dsTemp.numTemp; i++ )
    {
      Serial.print("Temp input ");
      Serial.print(dsTemp.SerialNum(i));
      Serial.print(" value ");
      Serial.println(dsTemp.ftemps[i]);
    }
    dsTime = millis();
  }
}
