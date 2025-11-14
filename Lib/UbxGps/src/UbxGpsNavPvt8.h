#ifndef UBXGPSNAVPVT8_H_INCLUDED
#define UBXGPSNAVPVT8_H_INCLUDED

#include <UbxGps.h>

class UbxGpsNavPvt8 : public UbxGps
{
public:
    struct __attribute__((packed)) {
        // Type         Name            Unit  Description (Scaling)
        uint8_t   ubxHdr[4];  // ublox header data
        uint32_t  iTOW;       //  ms    GPS time of week of the navigation epoch. See the description of iTOW for
                                    //        details.
        int16_t   year;       //  y     Year (UTC)
        uint8_t   month;      //  month Month, range 1..12 (UTC)
        uint8_t   day;        //  d     Day of month, range 1..31 (UTC)
        uint8_t   hour;       //  h     Hour of day, range 0..23 (UTC)
        uint8_t   min;        //  min   Minute of hour, range 0..59 (UTC)
        uint8_t   sec;        //  s     Seconds of minute, range 0..60 (UTC)
        uint8_t   valid;      //        Validity flags (see graphic below)
        uint32_t  tAcc;       //  ns    Time accuracy estimate (UTC)
        int32_t   nano;       //  ns    Fraction of second, range -1e9 .. 1e9 (UTC)
        uint8_t   fixType;    //        GNSSfix Type:
                              //        0: no fix
                              //        1: dead reckoning only
                              //        2: 2D-fix
                              //        3: 3D-fix
                              //        4: GNSS + dead reckoning combined
                              //        5: time only fix
        uint8_t   flags;      //        Fix status flags (see graphic below)
        uint8_t   flags2;     //        Additional flags (see graphic below)
        int8_t    numSV;      //        Number of satellites used in Nav Solution
        int32_t   lon;        //  deg   Longitude (1e-7)
        int32_t   lat;        //  deg   Latitude (1e-7)
        int32_t   height;     //  mm    Height above ellipsoid
        int32_t   hMSL;       //  mm    Height above mean sea level
        uint32_t  hAcc;       //  mm    Horizontal accuracy estimate
        uint32_t  vAcc;       //  mm    Vertical accuracy estimate
        int32_t   velN;       //  mm/s  NED north velocity
        int32_t   velE;       //  mm/s  NED east velocity
        int32_t  velD;        //  mm/s  NED down velocity
        int32_t  gSpeed;      //  mm/s  Ground Speed (2-D)
        int32_t  headMot;     //  deg   Heading of motion (2-D) (1e-5)
        uint32_t  sAcc;       //  mm/s  Speed Accuracy Estimate
        uint32_t  headAcc;    //  deg   Heading accuracy estimate (both motion and vehicle) (1e-5)
        uint16_t  pDOP;       //        Position DOP (0.01)
        uint16_t  flags3;     //        Additional flags (see graphic below)
        uint8_t   rsrvd1[4];  //        Reserved
        uint32_t  headVeh;    //  deg   Heading of vehicle (2-D), this is only valid when headVehValid is set,
                                    //        otherwise the output is set to the heading of motion (1e-5)
        int16_t   magDec;     //  deg   Magnetic declination. Only supported in ADR 4.10 and later. (1e-2)
        uint16_t  magAcc;     //  deg   Magnetic declination accuracy. Only supported in ADR 4.10 and later. (1e-2)

    } NavPvt8;


  UbxGpsNavPvt8() : UbxGps()
  {
    this->setHeader(0x1, 0x7);  // 0x1 0x7 is packet NavPvt
    this->setChanLength(0);     // no channels for this package type
    this->setDataPtr(&NavPvt8);
  }
};

#endif
