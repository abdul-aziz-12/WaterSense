
#include<SoftwareSerial.h>

const byte TX= 2;
const byte  RX= 3;
int trig = 11;
int echo = 12;
 
SoftwareSerial uart(TX,RX); // HC-12 TX Pin, HC-12 RX Pin

void setup()
{
  Serial.begin(9600);
  delay(5);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT); 
  uart.begin(9600);
  
}

void loop()
{
  float time1 = 0, h = 0;
  
  // Transmitting pulse
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  // Waiting for pulse
  time1 = pulseIn(echo, HIGH);
  
  // Calculating distance 
  h = time1*.034/2;
 
  h = 13 - h;  // water height

  
  String h1 = String (h);
  String transmit = String('s'+h1+'e');
  uart.println(transmit);
  delay(5);
  Serial.println(transmit);
  delay(1000);
//6s delay i.e sends value of h(height of water) after every 6s 
  
}
