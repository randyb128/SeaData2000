/*
  433mhz receiver module test
  Displays value, bit length and protocol  
  Setup for the Data input line on the SeaData2000 modular NMEA2000 board
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();
#define   DATA_LINE   25      // GPIO 25 is data line

void setup() {
  Serial.begin(115200);
  //pinMode(DATA_LINE, INPUT_PULLUP);
  mySwitch.enableReceive(DATA_LINE);  // ESP32 D25 Interrupt
}

void loop() {
  //Serial.print(digitalPinToInterrupt(2));
  //Serial.println(mySwitch.getIntCount());
  if (mySwitch.available()) {
    Serial.print("Received: ");
    Serial.print( mySwitch.getReceivedValue() );
    Serial.print(" / ");
    Serial.print( mySwitch.getReceivedBitlength() );
    Serial.print("bit ");
    Serial.print(" Protocol: ");
    Serial.println( mySwitch.getReceivedProtocol() );

    mySwitch.resetAvailable();
  }
}
