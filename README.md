# SeaData2000 ⛵
Source code repository for the SeaData 2000 marine interface board
## What is SeaData2000
SeaData2000 is a NMEA 2000 capable interface board based on an Arduino ESP32 module.  The board provides many pluggable features that can be used in a multitude of applications on any vessel.
## History 
The board development stems from an issue I had with a brand new RayMarine Element 12S GPS chart plotter.  Unbeknownst to me, this chart plotter has a known issue where the internal GPS signal appears to drop out unexpectedly.  If for instance you had selected a waypoint heading, the unit will occasionlly claim the GPS signal is lost and the heading is gone!  This issue would happen many times per day and is very annoying and even outright dangerous at times. The RayMarine engineers blamed it on mounting location of the 12S but mine is exposed to the sky on a fiberglass sailboat.  Searching the web, many users have this same issue with the Element series of chart plotters from RayMarine.  I have over 20 years experience with different GPS modules and IMHO I believe there's a software and or hardware issue in the Raymarine Element series that they don't want to or can't address.  Raymarine's solution is to purchase a NMEA 2000 GPS antenna and use that NMEA 2000 data as the source for the 12S as opposed to the internal GPS.  The Raymarine NMEA 2000 GPS module solution is the RS150 and is over $300 USD and needs to be mounted and wired to NMEA2000 bus.  I decided that I could build a NMEA 2000 GPS device that could also be used for other things on the boat.  I wanted to measure some temperatures and possibly control some outputs. I also installed a RayMarine Evolution autopilot and would love to have remote control for that unit.  I also thought it would be nice to have a digital compass heading and while I was at it why not a gyro to get the attitude of the boat. I thought why not share what I've done with others so here you go.
## Design Concepts
The SeaData2000 is based on an ESP32 WROOM Arduino module.  This provides both Bluetooth BLE and WiFi capabalities.  In the design, the idea was to keep costs and power consumption down.  You can select which pluggable modules you want to use for your application depending on your design requirements.  The modules are easy to find from various North American or offshore sources. This allows the SeaData2000 to be very efficient from a power standpoint has you only populate the modules you need for your application.  The main processor module is an ESP32 WROOM
Example code for the SeaData2000 NMEA interface is based on the library by Timo Lappalainen found [here](

## Arduino IDE Quick Start Guide :bulb:
1. Install [Arduino IDE](https://www.arduino.cc/en/software)
2. Install [Arduino lastest ESP32](https://docs.espressif.com/projects/arduino-esp32/en/latest/)
3. Copy all folders in to your Arduino library folder (e.g. C:\Users\YourLoginName\Documents\Arduino\libraries)
4. Open ArduinoIDE,  

> \[!IMPORTANT]
>
> When opening the Arduino IDE, you may be prompted that there are new libraries that can be updated.  Please do not click update.  There is no guarantee that it can be compiled, run, etc..


| Syntax | Description |
| ------ | ----------- |


