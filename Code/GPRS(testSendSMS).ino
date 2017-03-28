unsigned char buffer[64];  // buffer array for data receive over serial port
int count=0;               // counter for buffer array 

String outMessage = "This is working perfectly, but your bike is gone! sorry Dude!!";
//String destinationNumber1= "+17606981122";
String destinationNumber2= "+17606981143";

void SIM900Power()
// software equivalent of pressing the GSM shield "power" button
{
  digitalWrite(9, HIGH);
  delay(5000);
  digitalWrite(9, LOW);
  delay(5000);
}
void setup()
{
  Serial1.begin(9600);
  //Serial.begin(9600);
  SIM900Power();
  delay(5000); // give time to log on to network.
  Serial1.print("AT+CMGF=1\r");
  delay(1000);
  Serial1.println("AT + CMGS = \"" + destinationNumber2 + "\"");
  delay(1000);
  Serial1.print(outMessage);
  delay(1000);
  Serial1.write((char)26); //ctrl+z
  delay(10000);
  SIM900Power();
  
}

void loop()
{

  
  if (Serial1.available())
  {
    while(Serial1.available())
    {
      buffer[count++]=Serial1.read();
      if(count == 64)break;
    }
    Serial.write(buffer,count);
    clearBufferArray();
    count = 0;
  }
  if (Serial.available())
    Serial1.write(Serial.read());
}

void clearBufferArray()
{
  for (int i=0; i<count;i++)
  {
    buffer[i]=NULL;
  }
}
