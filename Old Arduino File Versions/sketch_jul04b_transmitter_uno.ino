
#include<SoftwareSerial.h>

const byte HC12_TX= 2;
const byte  HC12_RX= 3;
int trig = 11;
int echo = 12;
 
SoftwareSerial HC12(HC12_TX,HC12_RX); // HC-12 TX Pin, HC-12 RX Pin

void setup()
{
  Serial.begin(9600);
  delay(5);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT); 
  HC12.begin(9600);
  
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
  HC12.println(transmit);
  delay(5);
  Serial.println(transmit);
  delay(2000);
//6s delay i.e sends value of h(height of water) after every 6s 
  
}
