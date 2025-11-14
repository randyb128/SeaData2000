#ifndef UBXGPS_H_INCLUDED
#define UBXGPS_H_INCLUDED

#include <Arduino.h>

const unsigned char UBXGPS_HEADER[] = {0xB5, 0x62};

class UbxGps
{
private:
    // Class properties.
    unsigned short size;     // this can be calculated when chanLength > 0
    unsigned short pos;
    unsigned char chanLength = 0;
    unsigned char checksum[2];
    boolean match;    // does class and id match?

    // Headers (common).
    unsigned char headerClass;
    unsigned char headerId;

    unsigned char* bufPtr;        // this will get set my inherited class

    void calculateChecksum()
    {
      checksum[0] = 0;
      checksum[1] = 0;
      for (int i = 0; i < size; i++)
      {
        checksum[0] += bufPtr[i];
        checksum[1] += checksum[0];
      }
    }

protected:
    UbxGps()
    {
      pos = 0;
    }

    void setDataPtr(void* p)
    {
      bufPtr = (unsigned char*)p;
    }

    void setHeader(unsigned char hclass, unsigned char hid)
    {
      headerClass = hclass;
      headerId = hid;
    }

    void setChanLength(unsigned char len)
    {
      chanLength = len;
    }

public:
  
  boolean ready(unsigned char b)
  {
    boolean Result = false;

    // Carriage is at the first or the second sync byte, should be equals.
    if (pos < 2)
    {
      if (b == UBXGPS_HEADER[pos])
      {
        pos++;
        size = 12;    // minimum size of message
        match = true;   // class and id match for now
      }
      // Reset if not.
      else
      {
        if (b == UBXGPS_HEADER[0])      // check if out of sync
          pos = 1;
        else
          pos = 0;
      }
    }
    // Sync with header after success.
    else
    {
      // Put the byte read to a particular address of this object which depends on the carriage position.
      if (pos < (size + 2) && match)
      {
          bufPtr[pos - 2] = b;
      }

      // Move the ptr
      pos++;

      switch (pos)
      {
        case 3:     // check class
          if (b != headerClass)
            match = false;
          break;
        case 4:
          if (b != headerId)
            match = false;
          break;
        case 5:
          // get lsb of length
          size = b;
          break;
        case 6:
          // get msb of length
          size += (b << 8);
          size += 4;    // we're including 4 extra bytes that are not in the packet length
          break;
      }

      if(pos == size + 3 && match) {    // only calculate checksum if this class and id matches what we're looking for
        calculateChecksum();       // calculate checksum
        // Reset if not equal to 1st checksum
        if (b != checksum[0])
        {
          if (b == UBXGPS_HEADER[0])      // in case we're out of sync
            pos = 1;
          else
            pos = 0;
        }
      } else if (pos == size + 4) {
        // Reset the carriage.
        pos = 0;
        // The readings are correct and filled the object, return true.
        if (b == checksum[1] && match)
          Result = true;      // success
      }
    }

    return Result;
  }
};

#endif
