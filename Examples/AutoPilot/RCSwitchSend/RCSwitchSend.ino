/*
  SeaData 2000 433mhz transmitter module example  rb May 13,2025

  Commands are: 'N' prompts to enter starting code number
  'A' - continuously transmits code number
  'B' - transmits code number + 1
  'C' - transmits code number + 2
  'D' - transmits code number + 3
  'E' - transmits code number + 4
  'F' - transmits code number + 5
  NOTE: You must solder on an antenna to the 433mhz transmitter module in order to program fobs

  NOTE: You don't want to purchase a rolling code style FOB but when that mentions cloning for copying.
  
  If you purchase your FOBs on Amazon, some of the vendors provide instructions on how to program the FOB.
  Here's what we've found works for 2 different styles of FOBs we purchased from both Amazon and off shore sources.

  To program a FOB with your new codes, you must first clear out the existing codes.
  They're many FOBs that look a like.  At the time of this writing, we've seen 2 different types of FOBs.  
  They look identical (see our README.md in the AutoPilot section for an image) but one distinguishing feature we've seen is the buttons.  There are 2 styles of buttons, flat and pan head.

  For pan head style FOBs, start by holding A & B buttons at the same time until the LED flashes.
  Next, release only the B button and press it 3 times.  The LED should blink.
  Release the A button.
  Now when you press any of the A-D buttons, the LED will not light.  The pan head type FOB is now ready for programming.

  For flat head button style FOBs, press the A & B buttons at the same time until the LED starts flashing continuously.
  Release both the A & B buttons.
  Now when you press any of the A-D buttons, the LED will light, but for less than one second.
  The flat head type FOB is now ready for programming.
  
  Run this sketch, enter your desired code using the "N" command then use the A-D command to select the button to program.
  The sketch will continuously send the selected button code sequence.  
  Hold the FOB very close to the antenna of the transmitter module.  Press and hold the button you wish to program.
  The LED will flash, but hold the button longer until the LED flashes again. (about 5 seconds or so)
  Release the button.  If programmed correctly, when you press the programmed button, the LED should stay lit while the button is depressed.
  Repeat for the other buttons you wish to program.

  NOTE: Only install and use the transmitter module with this sketch to program your FOBs.  
  The enable line is hardwired high so the transmitter is on all the time and will interfere with the 433mhz receiver module if installed.
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

//#define   TX_ENABLE   22      // tx enable line on SeaData2000 RXB12 module
#define   TX_LINE     23      // txmit line on SeaData2000 RXB12 module
#define   MSG_LEN     24      // message length of 24 bits
#define   PROTOCOL    1       // 433mhz protocol
#define   PULSE_LEN   356
#define   MAX_VALUE   16777216    // max 24 bits
#define   ENTRY_CMD   'N'

int dly = 0;                 // display delay
#define   STR_LEN     8      // max string length
char receivedChars[STR_LEN + 1];   // an array to store new code number string
unsigned long codeNumber = 1234L;      // code number to send
char mode;                     // current mode of operation A-E

void setup() {

  Serial.begin(115200);
  codeNumber = 1234L;         // random number to start with
  mode = 'A';
  // Transmitter is connected to Arduino Pin #13  
  mySwitch.enableTransmit(TX_LINE);
  //pinMode(TX_ENABLE, OUTPUT);    // en line as output
  //digitalWrite(TX_ENABLE, HIGH); // set it high
  
  // Optional set protocol (default is 1, will work for most outlets)
  mySwitch.setProtocol(PROTOCOL, PULSE_LEN);  

}

void recvNumValue() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    long val;
    
    while (Serial.available() > 0) {
        rc = Serial.read();

        if (rc != endMarker) {
            if (rc >= '0' && rc <= '9' && ndx < STR_LEN)
              receivedChars[ndx++] = rc;
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            if (strlen(receivedChars) > 0)
            {
              ndx = 0;
              val = atol(receivedChars);
              if (val < MAX_VALUE)
              {
                codeNumber = val;
                mode = 'A';     // start sending code 'A'
                dly = 50;       // update display immediately
              }
              else
                Serial.println("Number value is too large");
            }
            else
              Serial.println("Please enter a numeric string");
        }
    }
}

void SendCode(unsigned long code)
{
  if (dly >= 50) {
    Serial.print("Sending code key ");
    Serial.print(mode);
    Serial.print(" code ");
    Serial.println(code);
    dly = 0;
  }
  
  mySwitch.send(code, MSG_LEN); // send button
  dly++;
  delay(1);
}

void loop() {
  char rc;

  if (mode == ENTRY_CMD)
    recvNumValue();
  else
  {
    if (Serial.available())
    {
      rc = Serial.read();
      rc = toupper(rc);
      if (rc >= 'A' && rc <= 'F')
      {
        mode = rc;
        dly = 50;
      } 
      else if (rc == 'N')
      {
        Serial.println("Please enter code");
        mode = rc;
      }
      else
      {
        if (rc != '\n')   // ignore LF
          Serial.println("Invalid command");
      }
    }
    SendCode(codeNumber + (mode - 'A'));
  }
}
