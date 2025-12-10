/*
  Output Raw MPU6050 and QMC5883L data for calibration
  rb July 2,2025

  The data output is captured by the C# program RecordData then analyzed using Magneto1.2

  Sketch outputs compass and gyro data where the raw compass data starts with "m:" 
  and the raw gyro data starts with "a:"
  The RecordData program can be setup to capture one or the other then output a CSV file for 
  analysis via the Magneto program
*/
#include <I2Cdev.h>
#include <MPU6050.h>
#include <QMC5883LCompass.h>
 
QMC5883LCompass compass;

/* MPU6050 default I2C address is 0x68*/
MPU6050 mpu;
//MPU6050 mpu(0x69);         //Use for AD0 high
//MPU6050 mpu(0x68, &Wire1); //Use for AD0 low, but 2nd Wire (TWI/I2C) object.

/* OUTPUT FORMAT DEFINITION----------------------------------------------------------------------------------
- Use "OUTPUT_READABLE_ACCELGYRO" if you want to see a tab-separated list of the accel 
X/Y/Z and gyro X/Y/Z values in decimal. Easy to read, but not so easy to parse, and slower over UART.

- Use "OUTPUT_BINARY_ACCELGYRO" to send all 6 axes of data as 16-bit binary, one right after the other. 
As fast as possible without compression or data loss, easy to parse, but impossible to read for a human. 
This output format is used as an output.
--------------------------------------------------------------------------------------------------------------*/ 
#define OUTPUT_READABLE_ACCELGYRO
//#define OUTPUT_BINARY_ACCELGYRO

#define LED_BUILTIN 2     // D2 is LED

int16_t ax, ay, az;
int16_t gx, gy, gz;
bool blinkState;
long lastMilli;

void setup() {
  /*--Start I2C interface--*/
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin(); 
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif

  Serial.begin(115200); //Initializate Serial wo work well at 8MHz/16MHz
  delay(1000);    // wait for serial port to initialize

  /*Initialize device and check connection*/ 
  Serial.println("Initializing MPU...");
  mpu.initialize();
  mpu.setDLPFMode(5);   // set low pass filter to 10hz for accelerometer
  Serial.println("Testing MPU6050 connection...");
  if(mpu.testConnection() ==  false){
    Serial.println("MPU6050 connection failed");
    while(true);
  }
  else{
    Serial.println("MPU6050 connection successful");
  }

  /* Use the code below to change accel/gyro offset values. Use MPU6050_Zero to obtain the recommended offsets */ 
  Serial.println("Updating internal sensor offsets...\n");
  mpu.setXAccelOffset(0); //Set your accelerometer offset for axis X
  mpu.setYAccelOffset(0); //Set your accelerometer offset for axis Y
  mpu.setZAccelOffset(0); //Set your accelerometer offset for axis Z
  mpu.setXGyroOffset(0);  //Set your gyro offset for axis X
  mpu.setYGyroOffset(0);  //Set your gyro offset for axis Y
  mpu.setZGyroOffset(0);  //Set your gyro offset for axis Z
  /*Print the defined offsets*/
  Serial.print("\t");
  Serial.print(mpu.getXAccelOffset());
  Serial.print("\t");
  Serial.print(mpu.getYAccelOffset()); 
  Serial.print("\t");
  Serial.print(mpu.getZAccelOffset());
  Serial.print("\t");
  Serial.print(mpu.getXGyroOffset()); 
  Serial.print("\t");
  Serial.print(mpu.getYGyroOffset());
  Serial.print("\t");
  Serial.print(mpu.getZGyroOffset());
  Serial.print("\n");

  // Initialize compass device with i2c 0x0D address
  compass.setADDR(0xd);
  compass.setReset();
  compass.init();

  /*Configure board LED pin for output*/ 
  pinMode(LED_BUILTIN, OUTPUT);
  lastMilli = millis();
}

void loop() {
  long m = millis();
  if (m - lastMilli >= 1000)
  {
    /*Blink LED to indicate activity*/
    blinkState = !blinkState;
    digitalWrite(LED_BUILTIN, blinkState);
    lastMilli = m;
  }
  if (compass.readStatus() & 1)
  {
    compass.readEx();
    Serial.print("m:\t");
    Serial.print(compass.getXRaw()); Serial.print("\t");
    Serial.print(compass.getYRaw()); Serial.print("\t");
    Serial.println(compass.getZRaw());

    /* Read raw accel/gyro data from the module. Other methods commented*/
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    //mpu.getAcceleration(&ax, &ay, &az);
    //mpu.getRotation(&gx, &gy, &gz);

  /*Print the obtained data on the defined format*/
  #ifdef OUTPUT_READABLE_ACCELGYRO
    Serial.print("a:\t");
    float res = mpu.get_acce_resolution();
    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.println(az);
  #endif

  #ifdef OUTPUT_BINARY_ACCELGYRO
    Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
    Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
    Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
    Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
    Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
    Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
  #endif
  }
}
