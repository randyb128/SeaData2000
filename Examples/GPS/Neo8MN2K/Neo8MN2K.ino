/***********************************************************************//**
  Neo8MN2K.ino
  NMEA2000 library example. Send GNSS and Rapid Update position to the bus.

  This example uses the UbxGpsNavPvt8 libary to read the Neo8M GPS module
  Note: The module must be setup using the Neo8MConfig program for 115200 baud and UBX protocol
  
  Example does not yet fullfill all NMEA2000 requirements.
*/

#include <Arduino.h>
//#define N2k_SPI_CS_PIN 53    // If you use mcp_can and CS pin is not 53, uncomment this and modify definition to match your CS pin.
//#define N2k_CAN_INT_PIN 21   // If you use mcp_can and interrupt pin is not 21, uncomment this and modify definition to match your interrupt pin.
//#define USE_MCP_CAN_CLOCK_SET 8  // If you use mcp_can and your mcp_can shield has 8MHz chrystal, uncomment this.
#define ESP32_CAN_TX_PIN GPIO_NUM_16 // If you use ESP32 and do not have TX on default IO 16, uncomment this and and modify definition to match your CAN TX pin.
#define ESP32_CAN_RX_PIN GPIO_NUM_17 // If you use ESP32 and do not have RX on default IO 4, uncomment this and and modify definition to match your CAN TX pin.
//#define NMEA2000_ARDUINO_DUE_CAN_BUS tNMEA2000_due::CANDevice1  // If you use Arduino DUE and want to use CAN bus 1 instead of 0, uncomment this.
//#define NMEA2000_TEENSY_CAN_BUS 1 // If you use Teensy 3.5 or 3.6 and want to use second CAN bus, uncomment this.
#include <NMEA2000_CAN.h>  // This will automatically choose right CAN library and create suitable NMEA2000 object
#include <N2kMessages.h>
#include <UbxGpsNavPvt8.h>
#include <UbxGpsNavSat.h>
#include <time.h>

UbxGpsNavPvt8 pvt;      // UBX navigation message
UbxGpsNavPvt8 pvtTemp;  // build UBX data here

// pins for NEO GPS Module
static const int RXPin = 4, TXPin = 5;

#define GPS_BAUDRATE 115200

// List here messages your device will transmit.
const unsigned long TransmitMessages[] PROGMEM={129029L,129025L,0};

// Define schedulers for messages. Define schedulers here disabled. Schedulers will be enabled
// on OnN2kOpen so they will be synchronized with system.
// We use own scheduler for each message so that each can have different offset and period.
// Setup periods according PGN definition (see comments on IsDefaultSingleFrameMessage and
// IsDefaultFastPacketMessage) and message first start offsets. Use a bit different offset for
// each message so they will not be sent at same time.
tN2kSyncScheduler RapidScheduler(false,200,200);
tN2kSyncScheduler GNSSScheduler(false,1000,1010);

// *****************************************************************************
// Call back for NMEA2000 open. This will be called, when library starts bus communication.
// See NMEA2000.SetOnOpen(OnN2kOpen); on setup()
void OnN2kOpen() {
  // Start schedulers now.
  RapidScheduler.UpdateNextTime();
  GNSSScheduler.UpdateNextTime();
}

// *****************************************************************************
void setup() {
  // Set Product information
  NMEA2000.SetProductInformation("00000001", // Manufacturer's Model serial code
                                 100, // Manufacturer's product code
                                 "GNSS Neo8M device",  // Manufacturer's Model ID
                                 "1.2.0.21 (2022-09-30)",  // Manufacturer's Software version code
                                 "1.1.0.0 (2022-09-30)" // Manufacturer's Model version
                                 );
  // Set device information
  NMEA2000.SetDeviceInformation(112233, // Unique number. Use e.g. Serial number.
                                60, // Device function=Navigation. See codes on https://web.archive.org/web/20190531120557/https://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                145, // Device class=Sensor Communication Interface. See codes on https://web.archive.org/web/20190531120557/https://www.nmea.org/Assets/20120726%20nmea%202000%20class%20&%20function%20codes%20v%202.00.pdf
                                2040 // Just choosen free from code list on https://web.archive.org/web/20190529161431/http://www.nmea.org/Assets/20121020%20nmea%202000%20registration%20list.pdf
                               );
  // Uncomment 2 rows below to see, what device will send to bus. Use e.g. OpenSkipper or Actisense NMEA Reader                           
  //Serial.begin(115200);
  //NMEA2000.SetForwardStream(&Serial);
  // If you want to use simple ascii monitor like Arduino Serial Monitor, uncomment next line
  //NMEA2000.SetForwardType(tNMEA2000::fwdt_Text); // Show in clear text. Leave uncommented for default Actisense format.

  // If you also want to see all traffic on the bus use N2km_ListenAndNode instead of N2km_NodeOnly below
  NMEA2000.SetMode(tNMEA2000::N2km_ListenAndNode,22);
  //NMEA2000.SetDebugMode(tNMEA2000::dm_Actisense); // Uncomment this, so you can test code without CAN bus chips on Arduino Mega
  NMEA2000.EnableForward(false); // Disable all msg forwarding to USB (=Serial)
  // Here we tell library, which PGNs we transmit
  NMEA2000.ExtendTransmitMessages(TransmitMessages);
  // Define OnOpen call back. This will be called, when CAN is open and system starts address claiming.
  NMEA2000.SetOnOpen(OnN2kOpen);
  NMEA2000.Open();
  Serial1.begin(GPS_BAUDRATE, SERIAL_8N1, RXPin, TXPin);    // 2nd serial port for Neo8m module
}


// *****************************************************************************
void loop() {
  if(Serial1.available())
  {
    unsigned char b = Serial1.read();
    if (pvtTemp.ready(b))
      pvt.NavPvt8 = pvtTemp.NavPvt8;
  }

  SendGNSS();
  NMEA2000.ParseMessages();
}

// *****************************************************************************
void SendGNSS() {
  tN2kMsg N2kMsg;

  if ( RapidScheduler.IsTime() ) {    // every 200ms
    RapidScheduler.UpdateNextTime();
    double lon = pvt.NavPvt8.lon / 10000000.0;
    double lat = pvt.NavPvt8.lat / 10000000.0;
    SetN2kLatLonRapid(N2kMsg, lat, lon);
    NMEA2000.SendMsg(N2kMsg);
  }
 
  if ( GNSSScheduler.IsTime() ) {   // every 1000ms
    GNSSScheduler.UpdateNextTime();
    struct tm tim;
    tim.tm_hour = 0;
    tim.tm_min = 0;
    tim.tm_sec = 0;
    tim.tm_mday = 18;
    tim.tm_mon = 10;
    tim.tm_year = 2025 - 1900;
    time_t t = mktime(&tim);
    uint16_t days = t / 86400L;
    long lsecs = t % 86400L;
    double secs = lsecs;
    double lon = pvt.NavPvt8.lon / 10000000.0;
    double lat = pvt.NavPvt8.lat / 10000000.0;
    double alt = pvt.NavPvt8.hMSL / 1000.0;
    tN2kGNSSmethod gnssMethod;
    switch(pvt.NavPvt8.fixType)
    {
      case 2:   // 2d - fix
        gnssMethod = N2kGNSSm_GNSSfix;
        break;
      case 3:  // 3d - fix
        gnssMethod = N2kGNSSm_PreciseGNSS;
        break;
      default:
        gnssMethod = N2kGNSSm_noGNSS;
        break;
    }
    SetN2kGNSS(N2kMsg, 0xff, days, secs, lat, lon, alt, N2kGNSSt_GPS, gnssMethod, pvt.NavPvt8.numSV, 0, pvt.NavPvt8.pDOP / 100.0);
    NMEA2000.SendMsg(N2kMsg);
  }
 
}

