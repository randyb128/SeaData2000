/*
  SeaData 2000 Input/Output Test module  rb July 3,2025
  MQTT publish to 
  output/led/chan0  payload "true" turns on LED otherwise off
  output/relay/chan0 payload "true" turns on 1st Relay
  output/relay/chan1 payload "true" turns on 2nd Relay

  subscribe to
  input/volts/chanX - sends channel X input in volts where X is 0-3
  input/raw/chanX   - sends channel X input in raw readings where X is 0-3

*/
#include <PicoMQTT.h>
#include <WiFi.h>

#ifndef WIFI_SSID
#define WIFI_SSID "YourSSID"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "YourPassword"
#endif

// uncomment this line to create your own WiFi server for MQTT
// you'll need to connect your MQTT client app to this SSID and password if uncommented
// otherwise, the ESP32 will attempt to connect to your boats WiFi network SSID defined above
//#define WIFI_SERVER

PicoMQTT::Server mqtt;

unsigned long last_publish_time = 0;

// input GPIO assignments
const int inputs[] = { 32, 33, 35, 34 };
#define IN_LEN  (sizeof(inputs)/sizeof(inputs[0]))

#define   LED_RED     2

#define   RELAY1      26
#define   RELAY2      27

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

void handler_led(const char * topic, const char * payload) 
{
    Serial.printf("Handler relay received message in topic '%s': %s\n", topic, payload);
    // only one LED for now so don't need to worry about the topic
    digitalWrite(outputs[0], strcmp(payload, "true") == 0);
}

void handler_relay(const char * topic, const char * payload) 
{
    Serial.printf("Handler led received message in topic '%s': %s\n", topic, payload);
    int chan = 1;
    if (topic[strlen(topic) - 1] == '1')    // last character of topic is the channel number
      chan = 2;
    digitalWrite(outputs[chan], strcmp(payload, "true") != 0);
}

void handler_auto(const char * topic, const char * payload) 
{
    Serial.printf("Handler auto received message in topic '%s': %s\n", topic, payload);
}

void setup() {
  Serial.begin(115200);
  while(! Serial);    // wait for serial port

  // initialize all the outputs
  for(int i = 0; i < OUT_LEN; i++)
  {
    pinMode(outputs[i], OUTPUT);
    digitalWrite(outputs[i], state[i]);
  }

#ifdef WIFI_SERVER
  // Create WiFi Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
  delay(2000);
  IPAddress ip = WiFi.softAPIP();
  Serial.print("WiFi server IP: ");
  Serial.println(ip);
#else
  // Connect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) { delay(1000); }
  delay(2000);
  Serial.printf("WiFi connected, IP: %s\n", WiFi.localIP().toString().c_str());
#endif

  // subscribe to relay and led messages
  mqtt.subscribe("output/led/#", handler_led);
  mqtt.subscribe("output/relay/#", handler_relay);
  mqtt.subscribe("auto/#", handler_auto);

  mqtt.begin();
  last_publish_time = millis();
  dly = millis();
}

void loop() {
  mqtt.loop();

  // every 500ms publish analog voltages
  if (millis() - dly >= 1000)
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

      String topic = "input/raw/chan" + String(i);
      String message = String(raw);
      mqtt.publish(topic, message);

      String topic2 = "input/volts/chan" + String(i);
      String message2 = String(volts);
      mqtt.publish(topic2, message2);
    }
    Serial.println();
  }
}

float CalcVoltage(int rawValue)
{
  return (rawValue / ADC_STEPS) * V_REF; // Convert to voltage
}
