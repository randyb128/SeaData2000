#ifndef UBXGPSNAVSAT_H_INCLUDED
#define UBXGPSNAVSAT_H_INCLUDED

#include <UbxGps.h>

class UbxGpsNavSat : public UbxGps
{
public:
    struct __attribute__((packed)) SVS {
      uint8_t   gnssId;     //        GNSS identifier
      uint8_t   svId;       //        Satellite Identifier
      uint8_t   cno;        // dBHz   Carrier to noise ratio (signal strength)
      int8_t    elev;       // deg    Elevation range (range: +/-90) unknown if out of range
      int16_t   azim;       // deg    Azimuth (range 0-360)
      int16_t   prRes;      // m      Pseudorange residual
      uint32_t  flags;      //        Bitmask
    };

    struct __attribute__((packed)) {
      // Type         Name            Unit  Description (Scaling)
      uint8_t   ubxHdr[4];  // ublox header data
      uint32_t  iTOW;       //  ms    GPS time of week of the navigation epoch. See the description of iTOW for
      uint8_t   version;    //        Message version (0x01 for this version)
      uint8_t   numSvs;     //        Number of satellites
      uint8_t   resrvd1[2]; //        Reserved
      SVS       svs[255];   //        allow room for maximum number of SVs
    } NavSat;


  UbxGpsNavSat() : UbxGps()
  {
    this->setHeader(0x1, 0x35);    // 0x1 0x35 is packet NavSat
    this->setChanLength(sizeof(SVS));     // size of svs info
    this->setDataPtr(&NavSat);
  }
};

#endif
