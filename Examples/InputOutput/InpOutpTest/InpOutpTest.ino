/*
  SeaData 2000 Input/Output Test module  rb July 3,2025

  Commands are: 
  'R' - turn on/off red LED
  '1' - turn on/off relay 1
  '2' - turn on/off relay 2

  The program will display the status of the LED's when they change and also the status of the inputs every 500ms
*/

// input GPIO assignments
const int inputs[] = { 26, 27, 35, 34 };
#define IN_LEN  (sizeof(inputs)/sizeof(inputs[0]))

#define   LED_RED     2

#define   RELAY1      32
#define   RELAY2      33

const char cmds[] = { 'R', '1', '2' };
const uint8_t outputs[] = { LED_RED, RELAY1, RELAY2 };
String names[] = { "Red LED", "Relay 1", "Relay 2" };
// initial state of outputs as well as tracking of on/off
bool state[] = { false, true, true };

#define OUT_LEN (sizeof(outputs)/sizeof(outputs[0]))

// 12 bit A/D converter
const float ADC_STEPS = 4096;
// reference voltage is 3.3 volts
const float V_REF = 3.3;
float CalcVoltage(int rawValue);
long dly;

void setup() {
  Serial.begin(115200);

  // initialize all the outputs
  for(int i = 0; i < OUT_LEN; i++)
  {
    pinMode(outputs[i], OUTPUT);
    digitalWrite(outputs[i], state[i]);
  }

  dly = millis();
}

void loop() {
  char rc;

  // every 500ms display analong voltages
  if (millis() - dly >= 500)
  {
    dly = millis();
    for(int i = 0; i < IN_LEN; i++)
    {
      Serial.print("Input");
      Serial.print(i + 1);
      Serial.print(" Counts ");
      int raw = analogRead(inputs[i]);
      Serial.print(raw);
      float volts = CalcVoltage(raw);
      Serial.print(" Voltage ");
      Serial.println(volts);
    }
    Serial.println();
  }

  // on data entry
  if (Serial.available())
  {
    rc = Serial.read();
    rc = toupper(rc);
    
    for(int i = 0; i < OUT_LEN; i++)
    {
      if(rc == cmds[i])
      {
        state[i] = !state[i];
        // Note: zero or false turns on the output
        digitalWrite(outputs[i], state[i]);
        Serial.print(names[i]);
        // LED's high turns them on, Relays are opposite, zero or low turns them on
        if (i < 3)
          Serial.println(state[i] ? " On" : " Off");
        else
          Serial.println(state[i] ? " Off" : " On");
        break;
      }
    }
  }
}

float CalcVoltage(int rawValue)
{
  return (rawValue / ADC_STEPS) * V_REF; // Convert to voltage
}
