/*
 use this to communicate with the GPS Neo-8M module via UBlox u-center
 You can also see the data on the Serial Console of the Arduino IDE provided the device is in NMEA 0183 output mode
*/

// pins for NEO-8M GPS Module
static const int RXPin = 21, TXPin = 22;

#define COMPUTER_BAUDRATE 115200
// if you've used the Neo8MConfig program and have set the protocol to UBX then you'll need to change the GPS_BAUDRATE to 115200
#define GPS_BAUDRATE 9600

void setup() {
  Serial.begin(COMPUTER_BAUDRATE);
  Serial1.begin(GPS_BAUDRATE, SERIAL_8N1, RXPin, TXPin);
}

void loop() {
    // If there is data from the GPS receiver, read it and send it to the computer or vice versa.
    if (Serial1.available())
    {
        Serial.write(Serial1.read());
    }

    if (Serial.available())
    {
        Serial1.write(Serial.read());
    }
}
