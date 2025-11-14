// DSTemp.h - header for Dallas I2C-to-onewire bridge IC 
//
// Started: Nov 13, 2025  Randy Bolton
//
//
// Requires DS2482 library by Joe Young 
//
// This library specifically supports up to 6 DS18B20 temperature sensors 
// 
//
#ifndef DSTEMP_HDR
#define DSTEMP_HDR

#include <Wire.h>
#include <Arduino.h>
#include "DS2482.h"

// DS18B20 temp sensor definitions
// ROM cmds
#define CSRCH 0xF0      //Search ROM command
#define CREAD 0x33      //Read ROM command
#define CMTCH 0x55      //Match ROM command
#define CSKRM 0xCC      //Skip ROM command
#define CASCH 0xEC      //Alarm Search command
//device function cmds
#define CCVRT 0x44      //Convert temperature
#define CWSPD 0x4E      //Write scracthpad
#define CRSPD 0xBE      //Read scratchpad
#define CCYPD 0x48      //Copy scratchpad
#define CRCEE 0xB8      //Recall EEPROM
#define CRPWR 0XB4      //Read power supply

enum DSErrCode {
    DS_OK,
    DS_BRIDGE_CHIP,
    DS_FAIL_CONFIG,
    DS_BUS_RESET,
    DS_SHORT_DETECTED,
    DS_NO_DEVICES
};

#define MAXID 6       // maximum number of one-wire serial numbers (temperature devices)
class DSTemp 
{
    public:
        DSTemp( uint8_t _i2cAdr );				// constructor receives DS2482 I2C address
        void begin();           // startup
        void loop();            // called by main .ino file

        bool short_detected;
        float ftemps[MAXID];     //storage for returned temperature floats, temperature to 2 decimals
        int numTemp;              // number of discovered devices
        DSErrCode lastError;            // set by begin() and loop() if a problem is detected
        bool fahrenheit = false;           // defaults to false, set to true to return ftemps in fahrenheit
        bool sortList = true;          // sort the list of sna's, defaults to true
        byte sna[MAXID][8];   //storage for discovered one-wire devices
        String SerialNum(int ndx);  // return the temperature device serial number as a string
    private:
        DS2482 *ds2482;        // pointer to Joe Young's DS2482 instantiated class
        int state = 0;
        unsigned long lTime;
        float CtoF(float c);
}; //class DSTemp

#endif
