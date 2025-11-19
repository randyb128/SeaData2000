## Auto Pilot

These example sketches are for the NMEA 2000 autopilot.  The autopilot consists of a 433mhz receiver that can be used to control the NMEA 2000 autopilot using the key FOB shown below (or equivalent) and the IoT MQTT App on your phone.  **Note:** the NMEA 2000 aout pilot code has only been tested with a RayMarine EV1 autopilot.  The auto pilot MQTT phone app connects to the SeaData 2000 via WiFi.  If your boat already has a WiFi network, then the SeaData 2000 can be configured to connect to that network. If you don't have a WiFi network, the ESP32 can be configure in Access Point mode.  This will create a stand-alone local WiFI network that you can connect to with your phone(s).  If you launch the IoT MQTT app and configure it (see the MQQTTSettings folder) there is an "AutoPilot" panel that you can use to control your autopilot remotely.

<div align="left" markdown="1">
  <img src=".github/keyfob.jpg" alt="Key Fob" width="200"/>
</div>

## RCSwitchSend

This example sketch is used to program the key fob pictured above.  It uses the Serial Monitor command line to program the FOB.  The commands are as follows:
- 'N' prompts to enter starting code number
- 'A' - continuously transmits code number
- 'B' - transmits code number + 1
- 'C' - transmits code number + 2
- 'D' - transmits code number + 3
- 'E' - transmits code number + 4
- 'F' - transmits code number + 5

**Special NOTE!** You must solder on an antenna to the 433mhz transmitter module in order to program fobs.  The FOB's receiver is not very sensitive and you will not have any luck programming without it.





