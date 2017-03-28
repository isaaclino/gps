#include <SoftwareSerial.h>
#include "FPS_GT511C3.h"
#include <TinyGPS.h>
#include<Wire.h>

/* This sample code demonstrates the normal use of a TinyGPS object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
const int MPU_addr=0x68;  // I2C address of the MPU-6050
TinyGPS gps;
FPS_GT511C3 fps(10, 11);
SoftwareSerial ss(12, 13);//11(tx)12(rx)
int16_t Ax=0,Ay=0,Az=0,Gx=0,Gy=0,Gz=0;
int16_t AcX=0,AcY=0,AcZ=0,Tmp=0,GyX=0,GyY=0,GyZ=0;
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
float flat=0, flon=0;
bool change = false;
void Stolen();
int led= 52;

unsigned char buffer[64]; // buffer array for data receive over serial port
int count=0;               // counter for buffer array 
static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);
String outMessage = "www.google.com/maps/place/";
char symbol = 0xB0; //degree symbol for google link
String destinationNumber2= "+17606981143";
void clearBufferArray();
void SIM900Power();
int stat=0;
void setup()
{
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);
  Wire.endTransmission(true);
  pinMode(led,OUTPUT);
 // ss.begin(9600);
//  Serial.begin(9600);
  Serial1.begin(9600);
  SIM900Power();
  delay(6000); 
  fps.Open();
  fps.SetLED(true);
}
void loop(){
  //Serial.println("bein stolen");
      digitalWrite(led,HIGH);
  if (stat==0)
  {
      
    if (fps.IsPressFinger())
      {
        fps.CaptureFinger(false);
        int id = fps.Identify1_N();
        if (id <200)
        {
         // Serial.print("Verified ID:");
          //Serial.println(id);
    // If the fingerprint is true (blink 6 times fast)
           fps.SetLED(true); // turn on the LED inside the fps
          delay(50);
          fps.SetLED(false);// turn off the LED inside the fps
          delay(50);
          fps.SetLED(true); // turn on the LED inside the fps
          delay(50);
          fps.SetLED(false);// turn off the LED inside the fps
          delay(50);
          fps.SetLED(true); // turn on the LED inside the fps
          delay(50);
          fps.SetLED(false);// turn off the LED inside the fps
          delay(50);
          fps.SetLED(true); // turn on the LED inside the fps
          delay(50);
          fps.SetLED(false);// turn off the LED inside the fps
          delay(50);
          fps.SetLED(true); // turn on the LED inside the fps
          delay(50);
          fps.SetLED(false);// turn off the LED inside the fps
          delay(50);
          fps.SetLED(true); // turn on the LED inside the fps
          delay(50);
          stat=1;
          Detect_Movement();
        }
        else
        {
          // if finger not found (wrong finger/person) blinks 2 times slow
         // Serial.println("Finger not found");
          
          fps.SetLED(true); // turn on the LED inside the fps
          delay(500);
          fps.SetLED(false);// turn off the LED inside the fps
          delay(500);
          fps.SetLED(true); // turn on the LED inside the fps
          delay(500);
          fps.SetLED(false);// turn off the LED inside the fps
          delay(500);
          fps.SetLED(true); // turn on the LED inside the fps
           delay(500);
           
        }
    }
    else
    {
      //Serial.println("Please press finger");
  
      // continuos blinking to wait for activation/arm 
        fps.SetLED(true); // turn on the LED inside the fps
        delay(100);
        fps.SetLED(false);// turn off the LED inside the fps
        delay(100);
        fps.SetLED(true); // turn on the LED inside the fps
    }
  }
  else if (stat==1)
  {
    if (fps.IsPressFinger())
      {
        digitalWrite(led,HIGH);
        fps.CaptureFinger(false);
        int id = fps.Identify1_N();
        if (id <200)
        {
         // Serial.print("Verified ID:");
          //Serial.println(id);
    // If the fingerprint is true (blink 6 times fast)
           fps.SetLED(true); // turn on the LED inside the fps
          delay(50);
          fps.SetLED(false);// turn off the LED inside the fps
          delay(50);
          fps.SetLED(true); // turn on the LED inside the fps
          delay(50);
          fps.SetLED(false);// turn off the LED inside the fps
          delay(50);
          fps.SetLED(true); // turn on the LED inside the fps
          delay(50);
          fps.SetLED(false);// turn off the LED inside the fps
          delay(50);
          fps.SetLED(true); // turn on the LED inside the fps
          delay(50);
          fps.SetLED(false);// turn off the LED inside the fps
          delay(50);
          fps.SetLED(true); // turn on the LED inside the fps
          delay(50);
          fps.SetLED(false);// turn off the LED inside the fps
          delay(50);
          fps.SetLED(true); // turn on the LED inside the fps
          delay(50);
          stat=0;
          SIM900Power();
          setup();
        }
        else
        {
          // if finger not found (wrong finger/person) blinks 2 times slow
         // Serial.println("Finger not found");
          
          fps.SetLED(true); // turn on the LED inside the fps
          delay(500);
          fps.SetLED(false);// turn off the LED inside the fps
          delay(500);
          fps.SetLED(true); // turn on the LED inside the fps
          delay(500);
          fps.SetLED(false);// turn off the LED inside the fps
          delay(500);
          fps.SetLED(true); // turn on the LED inside the fps
           delay(500);
           fps.SetLED(false);
           Stolen();
           loop();
        }
    }
    else
    {
      //Serial.println("Please press finger");
  
      // continuos blinking to wait for activation/arm 
        fps.SetLED(true); // turn on the LED inside the fps
        delay(100);
        fps.SetLED(false);// turn off the LED inside the fps
        delay(100);
        fps.SetLED(true); // turn on the LED inside the fps
        digitalWrite(led,LOW);
         Detect_Movement();
    }

  
  }
}


void Detect_Movement(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  //NEW SET OF COORDINATES
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  Ax=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  Ay=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  Az=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  Gx=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  Gy=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  Gz=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
//  if (Serial1.available())
//  {
//    while(Serial1.available())
//    {
//      buffer[count++]=Serial1.read();
//      if(count == 64)break;
//    }
//    Serial.write(buffer,count);
//    clearBufferArray();
//    count = 0;
//  }
//  if (Serial.available())
//    Serial1.write(Serial.read());
if((abs(Ax)-abs(AcX))>700 ||(abs(Ay)-abs(AcY))>700 || (abs(Az)-abs(AcZ))>700 || (abs(Gx)-abs(GyX))>700 ||(abs(Gy)-abs(GyY))>700 ||(abs(Gz)-abs(GyZ))>700 ) 
  {
     Stolen();
  }


    smartdelay(1000);
  }
void Stolen()
{
  
  unsigned long age, date, time, chars = 0;
  unsigned short sentences = 0, failed = 0;
  static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
  gps.f_get_position(&flat, &flon, &age);
 // print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
 // print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
  gps.stats(&chars, &sentences, &failed);
 // Serial.println();
 // Serial.print("going to send a message");
 // Serial.println();
  Send_SMS();
}
void Send_SMS ()
{
  unsigned long age, date, time, chars = 0;
  if(Serial.available())
  gps.f_get_position(&flat, &flon, &age);
  int degLa=flat;
float remainLa=(float)flat-degLa;
float minLa = 60 *remainLa;
//float secLa= 3600*(remainLa-(float)minLa/60);
String DEGLA= String(degLa,DEC);
String MINLA= String(minLa,DEC);
//String SECLA= String(secLa,DEC);
float temp=-1*flon;
int degLon=temp;
float remainLon=(float)temp-degLon;
float minLon = 60 *remainLon;
//float secLon= 3600*(remainLon-(float)minLon/60);//°
String DEGLON= String(degLon,DEC);
String MINLON= String(minLon,DEC);
//String SECLON= String(secLon,DEC);
  //SIM900Power();
  //delay(20000);//give time to log into network
  Serial1.print("AT+CMGF=1\r");
  delay(1000);
  Serial1.println("AT + CMGS = \"" + destinationNumber2 + "\"");
  delay(1000);
  //Serial.print(outMessage+DEGLA+"%20"+MINLA+"N,%20"+DEGLON+"%20"+MINLON+"W");
//  if (Serial1.available())
  Serial1.print(outMessage+DEGLA+"%20"+MINLA+"N,%20"+DEGLON+"%20"+MINLON+"W");
  delay(1000);
  Serial1.write((char)26); //ctrl+z
  delay(15000);
  //Serial.print("message sent");
 // SIM900Power();
}

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
//static void print_float(float val, float invalid, int len, int prec)
//{
//  if (val == invalid)
//  {
//    while (len-- > 1)
//      Serial.print('*');
//    Serial.print(' ');
//  }
//  else
//  {
//    Serial.print(val, prec);
//    int vi = abs((int)val);
//    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
//    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
//    for (int i=flen; i<len; ++i)
//      Serial.print(' ');
//  }
//  smartdelay(0);
//}
//
//static void print_str(const char *str, int len)
//{
//  int slen = strlen(str);
//  for (int i=0; i<len; ++i)
//    Serial.print(i<slen ? str[i] : ' ');
//  smartdelay(0);
//}
void SIM900Power()
// software equivalent of pressing the GSM shield "power" button
{
  digitalWrite(9, HIGH);
  delay(5000);
  digitalWrite(9, LOW);
  delay(5000);
  
}

void clearBufferArray()
{
  for (int i=0; i<count;i++)
  {
    buffer[i]=NULL;
  }
}
