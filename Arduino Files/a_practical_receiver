#include<SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<Wire.h>
#include<RTClib.h>
RTC_DS1307 RTC;
//--------------Your Data----------------//
const char* ssid = "Lenovo C2"; // <<<<<<<< Your Wi-Fi SSID 
const char* password = "507522d2fe1a"; // <<<<<<<< Your Wi-Fi Pasword
const char* mqtt_server = "35.192.49.64";//address

const byte RX = D0;
const byte TX = D4; 

int trig = D5;
int echo = D6; 

String readBuffer;
char incomingByte;
#define pump_pin D7


float up_high = 11;
float up_low = 3;
float down_high = 12;
float down_low = 4;  

int years = 2020;
int months = 2;
int date = 13;
int hours = 0;
int minutes = 0;
int seconds = 0;
String MotorState = "off";

WiFiClient espClient;
PubSubClient client (espClient);
SoftwareSerial uart(TX, RX);


void setup() {

  Serial.begin(9600);
  delay(5);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  uart.begin(9600);
  pinMode(pump_pin, OUTPUT);

    Wire.begin();
  RTC.adjust(DateTime(years, months, date, hours, minutes, seconds ));
  
 Serial.print("Connecting to ");
     Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());
    
}

void loop() {
DateTime now = RTC.now();
//for ultrasonic sensor
     float time2 = 0, hp = 0;
//for uart
  float h = 0;


readBuffer = "";
  boolean start = false;
  // Reads the incoming angle
  while (uart.available()) {             // If HC-12 has data
    incomingByte = uart.read();          // Store each icoming byte from HC-12
    delay(5);
    // Reads the data between the start "s" and end marker "e"
    if (start == true) {
      if (incomingByte != 'e') {
        readBuffer += char(incomingByte);    // Add each byte to ReadBuffer string variable
      }
      else {
        start = false;
      }
    }
    // Checks whether the received message statrs with the start marker "s"
    else if ( incomingByte == 's') {
      start = true; // If true start reading the message
    }
  }
  // Converts the string into integer
  delay(1000);
  h = readBuffer.toFloat();
  Serial.print("Water level in upper tank = ");
  Serial.print(h);
  Serial.println(" cm");
  
  
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  // Waiting for pulse
  time2 = pulseIn(echo, HIGH);
  
  // Calculating distance 
  hp = time2*.034/2;
 
  hp = 15 - hp;  // water height
  
  //if h<=2 ->turn motor on
  //if h>=10 ->turn off motor
  //check after every 1 min
 Serial.print("Water level in lower tank = ");
  Serial.print(hp);
  Serial.println(" cm");
  Serial.println("------------------------------------------");

 

if(hp<=down_low && MotorState == "on")
{
  digitalWrite(pump_pin,LOW);
  MotorState == "off";
  
  }
else if(hp>down_low && hp<down_high)
 {
    if(h < up_low && MotorState == "off"){
    digitalWrite(pump_pin,HIGH);
    RTC.adjust(DateTime(years, months, date, hours, minutes, seconds ));
    MotorState == "on";
    }
    else if (h > up_high && MotorState == "on"){
    digitalWrite(pump_pin,LOW); 
    MotorState == "off";
    }
  }
  
else if(hp>=down_high && MotorState == "off"){
digitalWrite(pump_pin,HIGH);
RTC.adjust(DateTime(years, months, date, hours, minutes, seconds ));
MotorState == "on";
}


    if((now.second() > 50)&&(h<up_low)&&(MotorState == "on"))
    {
      digitalWrite(pump_pin, LOW);
      MotorState = "Motor Stopped Working";
      delay(10000);

      while(1){

          //Print motor state on cloud and screen 
          Serial.println(MotorState);
          delay(10000);
        }
      }
      

  
delay(2000);

}
