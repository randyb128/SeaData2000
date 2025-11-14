# SeaData2000 Temperature Examples
The SeaData2000 board supports multiple Dallas DS18B20 temperature probes.  DS18B20 probes like the one's pictured below are readily available from Amazon or other offshore suppliers usually around $2-3 USD each.  There are 2 sets of screw terminals for 2 individual temperature probes but temperature probes can be wired in parallel.  The temperature library and source code supports up to 6 temperature probes but that can easily be changed in the code by editting the $Your Arduino Home Dir\libraries\DSTemp\DSTemp.h and changing the MAXID value.  The temperature probes require no extra modules to be installed.   

<div align="left" markdown="1">
  <img src=".github/TempProbe.jpg" alt="Temperature Probe" width="200"/>
</div>

# Source Code
**TempSimple** 
- this example requires at least one temperature probe to be attached to one of the 2 sets of screw terminals.  There is a dependency of the DS2482 library by Joe Young and our DSTemp library.  Temperature readings default to Celsius degrees by you can set the class property "fahrenheit" to "true" if desired.  
