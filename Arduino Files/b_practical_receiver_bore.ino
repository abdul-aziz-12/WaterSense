#include<SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include<ESP8266WebServer.h>
#include<RTCZero.h>
//--------------Your Data----------------//
const char* ssid = "Lenovo C2"; // <<<<<<<< Your Wi-Fi SSID 
const char* password = "507522d2fe1a"; // <<<<<<<< Your Wi-Fi Pasword
ESP8266WebServer server(80);
const byte RX = D2; //GPIO 4
const byte TX = D4; //GPIO 2
const byte buzzer_pin = D9;
const byte trig = D5;
const byte echo = D6; 
String readBuffer;
char incomingByte;
#define pump_pin D1
float up_high = 11;
float up_low = 3;
const byte seconds = 0;
const byte minutes = 0;
const byte hours = 0;
const byte day = 1;
const byte month = 1;
const byte year = 1;
String MotorState = "off";

WiFiClient client;
RTCZero rtc;

SoftwareSerial uart(TX, RX);


void setup() {

  Serial.begin(9600);
  delay(5);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  uart.begin(9600);
  pinMode(pump_pin, OUTPUT);
  pinMode(buzzer_pin, OUTPUT);
  digitalWrite(pump_pin, LOW);
  digitalWrite(buzzer_pin, LOW);

  rtc.begin();
  rtc.setHours(hours);
  rtc.setMinutes(minutes);
  rtc.setSeconds(seconds);

  // Set the date
  rtc.setDay(day);
  rtc.setMonth(month);
  rtc.setYear(year);

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


  server.begin();
  Serial.println("HTTP server started");
    
}

void loop() {

//for ultrasonic sensor
     float time2 = 0;
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
   
    
    if(h<up_low && MotorState == "off")
    {
      digitalWrite(pump_pin,HIGH);
      MotorState = "on";
      rtc.setSeconds(seconds); 
      
    }
    else if (h > up_high && MotorState == "on")
    {
      digitalWrite(pump_pin,LOW); 
      MotorState = "off";
    } 

    if((rtc.getSeconds()>50)&&(h<up_low)&&(MotorState == "on"))
    {
      digitalWrite(pump_pin, LOW);
      digitalWrite(buzzer_pin, HIGH);
      MotorState = "Motor Stopped Working";
      delay(10000);
      digitalWrite(buzzer_pin, LOW);

      while(1){

          //Print motor state on cloud and screen 
          Serial.println(MotorState);
          delay(10000);
        }
      }

          //Print motor state on cloud and screen 
  
delay(9000);

}
