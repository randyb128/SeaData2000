/*
  This utility sets up the Neo8M GPS module for Ublox UBX protocol at 10 samples per second using 115,200 baud rate
  Any extra messages are disabled including the Nmea 0183 output
  The utility supports 3 commands:
  "r" - restore the module back to default settings.  Nmea 0183 output at 9600 baud on most modules.
  "b" - change just the baud rate from 9600 to 115200
  "c" - configure the module for UBX binary protocol at 115200 baud and 10 samples per second
  Commands are issued via the Serial Monitor command line
  
  rb - May 14,2025
*/

#include <UbxGpsConfig.h>

#define COMPUTER_BAUDRATE 115200

UbxGpsConfig<HardwareSerial, HardwareSerial> *ubxGpsConfig = new UbxGpsConfig<HardwareSerial, HardwareSerial>(Serial2, Serial);
#define   RXD_PIN   4      // SeaData 2000 RX pin GPIO4
#define   TXD_PIN   5      // SeaData 2000 TX pin GPIO5

void setup()
{
    Serial.begin(COMPUTER_BAUDRATE);

    ubxGpsConfig->setPins(RXD_PIN, TXD_PIN);
    ubxGpsConfig->setMessage(UbxGpsConfigMessage::NavPvt);
    ubxGpsConfig->setRate(100);
    delay(5000);
    Menu();
}

void Menu()
{
  Serial.println("Commands: r - restore, c - configure, b - baudrate ");
}

void loop()
{
    // If there is data from the GPS receiver, read it and send it to the computer or vice versa.
    while (Serial.available())
    {
        char c = Serial.read();
        c = tolower(c);
        switch(c)
        {
          case 'r':
            Serial.println("Restoring settings");
            ubxGpsConfig->restore();
            break;
          case 'c':
            ubxGpsConfig->configure();
            break;
          case 'b':
            ubxGpsConfig->confBaudrate();
            break;
          default:
            if (c != '\n')
              Serial.println("Unknown command");
            break;
        }
        Menu();
    }
}
