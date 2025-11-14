#ifndef UBXGPSCONFIG_H_INCLUDED
#define UBXGPSCONFIG_H_INCLUDED

#include <Arduino.h>

// The default baudrate is determined by the GPS receiver manufacturer.
#define GPS_DEFAULT_BAUDRATE 9600

enum class UbxGpsConfigMessage
{
    NavPosecef = 0x01,
    NavPossllh = 0x02,
    NavPvt = 0x07,
    NavSol = 0x06,
};

// An array of possible baudrates that can be used by the GPS receiver, sorted descending to prevent excess Serial
// flush/begin after restoring defaults. You can uncomment values that can be used by your GPS receiver before the
// auto-configuration.
const long gpsPossibleBaudrates[] = {
    9600,
    115200,
};

template <class GpsSerial = HardwareSerial, class LogSerial = HardwareSerial>
class UbxGpsConfig
{
private:
    GpsSerial &gpsSerial;
    LogSerial &logSerial;

    UbxGpsConfigMessage message;
    unsigned short rate = 1000;
    int8_t TxPin = -1;
    int8_t RxPin = -1;

    void calcChecksum(byte *packet, int packetSize, byte *chksum)
    {
        // Set checksum bytes to null.
        chksum[0] = 0x00;
        chksum[1] = 0x00;

        logSerial.print("chk:");
        for (int j = 2; j < packetSize - 2; j++)
        {
            logSerial.printf(" %.2X", packet[j]);
            chksum[0] += packet[j];
            chksum[1] += packet[packetSize - 2];
        }
        logSerial.println();
    }

    void calcChecksum(byte *packet, int packetSize)
    {
      calcChecksum(packet, packetSize, packet + packetSize - 2);
    }

    // Print a packet to the log serial port in a hexadecimal form.
    void printPacket(byte *packet, byte len)
    {
        char temp[3];

        for (byte i = 0; i < len; i++)
        {
            sprintf(temp, "%.2X", packet[i]);
            logSerial.print(temp);

            if (i != len - 1)
            {
                logSerial.print(' ');
            }
        }

        logSerial.println();
    }

    // Send a packet to the GPS receiver.
    bool sendPacket(byte *packet, byte len)
    {
        for (byte i = 0; i < len; i++)
        {
            gpsSerial.write(packet[i]);
        }

//        printPacket(packet, len);
        return WaitForAck();
    }

    // save changes to BBR and Flash
    bool saveSettings()
    {
        // CFG-CFG packet buffer.
        byte packet[] = {
            0xB5, // sync char 1
            0x62, // sync char 2
            0x06, // class
            0x09, // id
            0x0d, // length
            0x00, // length
            0x00, // payload clearMask
            0x00, // 
            0x00, //
            0x00, //
            0x1F, // saveMask
            0x1F, // save all
            0x00, //
            0x00, //
            0x00, // loadMask
            0x00, // 
            0x00, //
            0x00, //
            0x03, // deviceMask (BBR,Flash)
            0x00, // CK_A
            0x00, // CK_B
        };
        byte packetSize = sizeof(packet);

        // Calculate checksum over the packet buffer excluding sync (first two) and checksum chars (last two).
        calcChecksum(packet, packetSize);

        return sendPacket(packet, packetSize);
    }

    // Send a packet to the GPS receiver to restore the default configuration.
    bool restoreDefaults()
    {
        // CFG-CFG packet.
        byte packet[] = {
            0xB5, // sync char 1
            0x62, // sync char 2
            0x06, // class
            0x09, // id
            0x0D, // length
            0x00, // length
            0xFF, // payload
            0xFF, // payload
            0x00, // payload
            0x00, // payload
            0x00, // payload
            0x00, // payload
            0x00, // payload
            0x00, // payload
            0xFF, // payload
            0xFF, // payload
            0x00, // payload
            0x00, // payload
            0x17, // payload
            0x2F, // CK_A
            0xAE, // CK_B
        };

        calcChecksum(packet, sizeof(packet));

        return sendPacket(packet, sizeof(packet));
    }

    // Send a set of packets to the GPS receiver to disable NMEA messages.
    bool disableNmea()
    {
        bool Result = true;

        // An array of two bytes for CFG-MSG packets payload.
        byte messages[][2] = {
            {0xF0, 0x0A},
            {0xF0, 0x09},
            {0xF0, 0x00},
            {0xF0, 0x01},
            {0xF0, 0x0D},
            {0xF0, 0x06},
            {0xF0, 0x02},
            {0xF0, 0x07},
            {0xF0, 0x03},
            {0xF0, 0x04},
            {0xF0, 0x0E},
            {0xF0, 0x0F},
            {0xF0, 0x05},
            {0xF0, 0x08},
            {0xF1, 0x00},
            {0xF1, 0x01},
            {0xF1, 0x03},
            {0xF1, 0x04},
            {0xF1, 0x05},
            {0xF1, 0x06},
        };

        // CFG-MSG packet buffer.
        byte packet[] = {
            0xB5, // sync char 1
            0x62, // sync char 2
            0x06, // class
            0x01, // id
            0x03, // length
            0x00, // length
            0x00, // payload (first byte from messages array element)
            0x00, // payload (second byte from messages array element)
            0x00, // payload (not changed in this case)
            0x00, // CK_A
            0x00, // CK_B
        };
        byte packetSize = sizeof(packet);

        // Offset to a place where payload starts.
        byte payloadOffset = 6;

        printf("count nmea %d\n", sizeof(messages) / 2);
        
        // Iterate over the messages array.
        for (byte i = 0; i < sizeof(messages) / 2; i++)
        {
            // Copy two bytes of payload to the packet buffer.
            for (byte j = 0; j < 2; j++)
            {
                packet[payloadOffset + j] = messages[i][j];
            }

            // Calculate checksum over the packet buffer excluding sync (first two) and checksum chars (last two).
            calcChecksum(packet, packetSize);

            if (! sendPacket(packet, packetSize))
            {
                Result = false;
                break;
            }
        }

        return Result;
    }

    // Send a packet to the GPS receiver to change the baudrate to 115200.
    bool changeBaudrate()
    {
        // CFG-PRT packet.
        byte packet[] = {
            0xB5, // sync char 1
            0x62, // sync char 2
            0x06, // class
            0x00, // id
            0x14, // length
            0x00, // length
            0x01, // payload
            0x00, // payload
            0x00, // payload
            0x00, // payload
            0xD0, // payload
            0x08, // payload
            0x00, // payload
            0x00, // payload
            0x00, // payload
            0xC2, // payload
            0x01, // payload
            0x00, // payload
            0x07, // payload
            0x00, // payload
            0x03, // payload
            0x00, // payload
            0x00, // payload
            0x00, // payload
            0x00, // payload
            0x00, // payload
            0xC0, // CK_A
            0x7E, // CK_B
        };

        calcChecksum(packet, sizeof(packet));
        return sendPacket(packet, sizeof(packet));
    }

    // Send a packet to the GPS receiver to change the frequency to 100 ms.
    bool changeFrequency(unsigned short rate)
    {
        // CFG-RATE packet.
        byte packet[] = {
            0xB5, // sync char 1
            0x62, // sync char 2
            0x06, // class
            0x08, // id
            0x06, // length
            0x00, // length
            0x64, // payload
            0x00, // payload
            0x01, // payload
            0x00, // payload
            0x01, // payload
            0x00, // payload
            0x0, // CK_A
            0x0, // CK_B
        };
        memmove(packet + 6, (void *) &rate, sizeof(rate));
        calcChecksum(packet, sizeof(packet));
        return sendPacket(packet, sizeof(packet));
    }

    // Send a packet to the GPS receiver to disable unnecessary channels.
    bool disableUnnecessaryChannels()
    {
        // CFG-GNSS packet.
        byte packet[] = {
            0xB5, // sync char 1
            0x62, // sync char 2
            0x06, // class
            0x3E, // id
            0x24, // length
            0x00, // length

            0x00, 0x00, 0x16, 0x04, 0x00, 0x04, 0xFF, 0x00, // payload
            0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x03, 0x00, // payload
            0x00, 0x00, 0x00, 0x01, 0x05, 0x00, 0x03, 0x00, // payload
            0x00, 0x00, 0x00, 0x01, 0x06, 0x08, 0xFF, 0x00, // payload
            0x00, 0x00, 0x00, 0x01,                         // payload

            0xA4, // CK_A
            0x25, // CK_B
        };

        return sendPacket(packet, sizeof(packet));
    }

    // Send a packet to the GPS receiver to enable NAV-X messages.
    bool enableNav()
    {
        // CFG-MSG packet.
        byte packet[] = {
            0xB5,          // sync char 1
            0x62,          // sync char 2
            0x06,          // class
            0x01,          // id
            0x03,          // length
            0x00,          // length
            0x01,          // payload
            (byte)message, // payload
            0x01,          // payload
            // TODO: Update checksum to fit message other than 0x07.
            0x0, // CK_A
            0x0, // CK_B
        };

        calcChecksum(packet, sizeof(packet));
        return sendPacket(packet, sizeof(packet));
    }

    bool WaitForAck()
    {
      bool Result = false;
      unsigned char checksum[2];
      unsigned char b;
      unsigned char buf[8];
      long ms;
      int pos = 0;
      unsigned short size = 8;
      bool match = false;

      ms = millis();
      // wait up to one second
      while(ms + 1000L > millis() && ! Result)
      {
        while(gpsSerial.available())
        {
          b = gpsSerial.read();
          logSerial.printf("%.2X ", b);
          // Put the byte read to a particular address of this object which depends on the carriage position.
          if (pos < (size + 2) && match && pos >= 2)
          {
              buf[pos - 2] = b;
          }

          // Move the ptr
          pos++;

          switch (pos)
          {
            case 1:
              if (b != 0xb5)    // 1st start of message
                pos = 0;
              break;
            case 2:
              if (b != 0x62)    // 2nd start of message
                pos = 0;
              break;
            case 3:             // check class of ack/nak, must be 0x5
              if (b != 0x5)
                match = false;
              break;
            case 4:
              if (b != 0x1 && b != 0x0)   // 0x1 is ack, 0x0 is nak
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
              if (size != 6)
                match = false;
              break;
          }

          if(pos == size + 3 && match) {    // only calculate checksum if this class and id matches what we're looking for
            calcChecksum(buf, size + 2, checksum);       // calculate checksum
            // Reset if not equal to 1st checksum
            //if (b != checksum[0])
            //  pos = 0;
          } else if (pos == size + 4) {
            // Reset the carriage.
            pos = 0;
            // The readings are correct and filled the object, return true.
            //if (b == checksum[1] && match)
              Result = true;      // success
            break;
          }
        }
      }
      logSerial.println();

      return Result;
    }

public:
    UbxGpsConfig(GpsSerial &gpsSerial, LogSerial &logSerial) : gpsSerial(gpsSerial),
                                                               logSerial(logSerial)
    {
        //
    }

    void setPins(int rxPin, int txPin)
    {
      RxPin = rxPin;
      TxPin = txPin;
    }

    void setMessage(UbxGpsConfigMessage _message)
    {
        message = _message;
    }

    void setRate(unsigned short _rate)
    {
        rate = _rate;
    }

    void restore()
    {
        // Restore the default GPS receiver configuration.
        for (byte i = 0; i < sizeof(gpsPossibleBaudrates) / sizeof(*gpsPossibleBaudrates); i++)
        {
            logSerial.print("Trying to restore defaults at ");
            logSerial.print(gpsPossibleBaudrates[i]);
            logSerial.println(" baudrate...");

            if (i != 0)
            {
                delay(100); // Little delay before the flush.
                gpsSerial.flush();
            }

            gpsSerial.begin(gpsPossibleBaudrates[i], SERIAL_8N1, RxPin, TxPin);
            logSerial.printf("Restore Baud: %d Result: %d\n", gpsPossibleBaudrates[i], restoreDefaults());
        }
        // Switch the GPS receiver serial configuration to the default baudrate.
        if (gpsPossibleBaudrates[sizeof(gpsPossibleBaudrates) / sizeof(*gpsPossibleBaudrates) - 1] != GPS_DEFAULT_BAUDRATE)
        {
            logSerial.print("Switching to the default baudrate which is ");
            logSerial.print(GPS_DEFAULT_BAUDRATE);
            logSerial.println("...");

            delay(100); // Little delay before the flush.
            gpsSerial.flush();
            gpsSerial.begin(GPS_DEFAULT_BAUDRATE, SERIAL_8N1, RxPin, TxPin);
        }
    }

    void confBaudrate()
    {
        logSerial.println("Changing to 115,200baud");
        for (byte i = 0; i < sizeof(gpsPossibleBaudrates) / sizeof(*gpsPossibleBaudrates); i++)
        {
            logSerial.print("Trying to set baud at ");
            logSerial.print(gpsPossibleBaudrates[i]);
            logSerial.println(" baudrate...");

            if (i != 0)
            {
                delay(100); // Little delay before the flush.
                gpsSerial.flush();
            }

            gpsSerial.begin(gpsPossibleBaudrates[i], SERIAL_8N1, RxPin, TxPin);
            logSerial.printf("changeBaudRate() to %d Result: %d\n", gpsPossibleBaudrates[i], changeBaudrate());
            logSerial.printf("saveSettings() Result %d\n", saveSettings());
        }
    }

    void configure()
    {
        logSerial.println("Starting auto-configuration...");

        // restore defaults trying different baud rate combinations
        confBaudrate();

        // Disable NMEA messages by sending appropriate packets.
        logSerial.println("Disabling NMEA messages...");
        logSerial.printf("Result %d\n", disableNmea());

        // Change measurement frequency rate to X ms.
        logSerial.print("Changing receiving frequency to ");
        logSerial.print(rate);
        logSerial.println(" ms...");
        logSerial.printf("Result %d\n", changeFrequency(rate));

        // Disable unnecessary channels like SBAS or QZSS.
        logSerial.println("Disabling unnecessary channels...");
        logSerial.printf("Result %d\n", disableUnnecessaryChannels());

        // Enable NAV-PVT messages.
        logSerial.println("Enabling NAV-PVT messages...");
        logSerial.printf("Result %d\n", enableNav());

        logSerial.println("Save settings NAV-CFG...");
        logSerial.printf("Result %d\n", saveSettings());

        logSerial.println("Auto-configuration is complete!");

        delay(100); // Little delay before the flush.
        gpsSerial.flush();
    }

    ~UbxGpsConfig()
    {
        gpsSerial.end();
    }
};

#endif
