#include "DSTemp.h"
#include <Wire.h>

// qsort requires you to create a sort function
static int sort_asc(const void *cmp1, const void *cmp2)
{
  // Need to cast the void * to int *
  uint64_t a = *((uint64_t *)cmp1);
  uint64_t b = *((uint64_t *)cmp2);
  // The comparison
 return a - b;
}

// create a local DS2482 object
DSTemp::DSTemp( uint8_t _i2cAdr ) 
{
	ds2482 = new DS2482(_i2cAdr);
} 

void DSTemp::begin( ) 
{
    for(int ix = 0; ix < MAXID; ix++)
    ftemps[ix] = 0;
    state = 0;
    lastError = DS_OK;
    if(! ds2482->DS2482_detect())
        lastError = DS_BRIDGE_CHIP;
    else if(! ds2482->DS2482_write_config(bit(APU)))
        lastError = DS_FAIL_CONFIG;
    //reset 1wire bus
    else if(! ds2482->OWReset())
        lastError = DS_BUS_RESET;
    else if(ds2482->short_detected)
        lastError = DS_SHORT_DETECTED;
    numTemp = 0;
    //search ROM with indefinite number of devices, up to MAXID
    if(ds2482->OWFirst()) 
    {
        bool found = true;
        for(int ix=0; ix<8; ix++) 
            sna[numTemp][ix] = ds2482->ROM_NO[ix];    //copy found to save area
        numTemp++;
        while(found && numTemp<MAXID ) {
            if( ds2482->OWNext()) 
            {
                for(int ix=0; ix<8; ix++) 
                    sna[numTemp][ix] = ds2482->ROM_NO[ix];
                numTemp++;
            } else {
                found = false;
            }
        } // while finding devices (fewer than MAXID)
    }
    if (sortList && numTemp > 1)        // sort the list of serial numbers if required
    {
        // qsort - last parameter is a function pointer to the sort function
        qsort(sna, numTemp, sizeof(sna[0]), sort_asc);
    } 
    if( numTemp == 0 )
        lastError = DS_NO_DEVICES;
}

float DSTemp::CtoF(float c)
{
    return (c * 9.0f / 5.0f) + 32.0f;
}

String DSTemp::SerialNum(int ndx)
{
    String Result = "";
    for(int i = 7; i >= 0; i--)
    {
        Result.concat(String(sna[ndx][i], HEX));
        if (i != 0)
            Result.concat(":");
    }
    return Result;
}

void DSTemp::loop()
{
    byte tmpMem[25];

    switch(state)
    {
        case 0:
            lTime = millis();
            state++;
            break;
        case 1:
            if (millis() - lTime >= 1000UL)
            {
                //measure with power
                ds2482->OWReset( );
                ds2482->OWWriteByte( CSKRM );     //skip ROM
                ds2482->OWWriteBytePower( CMD_WWBP ); //strong pullup, start conversion
                lTime = millis();
                state++;
            }
            break;
        case 2:
            if (millis() - lTime >= 1000UL)       // wait 1 second with strong pullup
            {
                ds2482->OWLevel( MODE_STANDARD );    //restore normal pullup
                for(int i = 0; i < numTemp ; i++ ) 
                {
                    ds2482->OWReset( );
                    tmpMem[0] = CMTCH;
                    for( byte ix=0; ix<8; ix++ )
                        tmpMem[ix+1] = sna[i][ix];
                    tmpMem[9] = CRSPD;
                    for( byte ix=0; ix<9; ix++ ) 
                        tmpMem[ix+10] = 0xFF;
                    ds2482->OWBlock( tmpMem, 19 );
                    uint8_t crc8 = 0;
                    for(int ix=0; ix<9; ix++ ) 
                        crc8 = ds2482->calc_crc8(tmpMem[ix+10]);       //accumulate crc for scratchpad bytes
                    Serial.print( ' ' );
                    Serial.println( crc8 );                      //report crc (should be zero)
                    if (crc8 == 0)
                    {
                        int itemp = ((int)(tmpMem[11])<<8) + (int)tmpMem[10];   //save raw temp value
                        ftemps[i] = itemp / 16.0;
                        if (fahrenheit)
                            ftemps[i] = CtoF(ftemps[i]);
                    }
                } //loop to report each found device
                state = 0;
            }
            break;
    }
}
