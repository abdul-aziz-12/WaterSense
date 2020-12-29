#include<SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include<ESP8266WebServer.h>
//--------------Your Data----------------//
const char* ssid = "Lenovo C2"; // <<<<<<<< Your Wi-Fi SSID 
const char* password = "507522d2fe1a"; // <<<<<<<< Your Wi-Fi Pasword
ESP8266WebServer server(80);
const byte HC12_TX = D2;
const byte HC12_RX = D3; 
int trig = D5;
int echo = D6; 
String readBuffer;
char incomingByte;
#define pump_pin D1
WiFiClient client;

SoftwareSerial HC12(HC12_TX, HC12_RX);


void setup() {

  Serial.begin(9600);
  delay(5);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  HC12.begin(9600);
  pinMode(pump_pin, OUTPUT);

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
     float time2 = 0, hp = 0;
//for HC12
  float h = 0;


readBuffer = "";
  boolean start = false;
  // Reads the incoming angle
  while (HC12.available()) {             // If HC-12 has data
    incomingByte = HC12.read();          // Store each icoming byte from HC-12
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
  Serial.println();
  
  
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
  Serial.println();
  Serial.println("------------------------------------------");

 

if(hp<=4.00)
{
  digitalWrite(pump_pin,LOW);
  }
else if(hp>4.00 && hp<12.00)
 {
    if(h < 3.00)
    digitalWrite(pump_pin,HIGH);
    else if (h > 11.00)
    digitalWrite(pump_pin,LOW); 
      
  }
  
else if(hp>=12.00)
digitalWrite(pump_pin,HIGH);


server.handleClient();
server.send(200,"text/html",SendHTML(h, hp));
  
delay(2000);

}

String SendHTML(float data1, float data2){

    String ptr = "<!DOCTYPE html><html>\n";
    
    ptr += "<head><meta =\"viewport\"content = \"width = device-width,initial-scale=1.0, user-scalable=no\">\n";
    ptr += "<meta http-equiv=refresh content=6>";
    ptr += "<title> Smart Water Motor Managment System </title>\n";
    ptr += "<style>html{font-family: Verdana;display: inline_block; margin: 0px auto;text-align: center;}\n";
    ptr += "body{margin-top: 50px;}h1{color:#444444; margin: 50px auto 30px;}\n";
    ptr += "p{font-size: 24px;color: #444444; margin-bottom: 10px;}\n";
    ptr += "</style>\n";
    ptr += "</head>\n";
    ptr += "<body>\n";
    ptr += "<div id=\"webpage\">\n";
    ptr += "<h1>ESP8266 NodeMCU Smart Water Management System</h1>\n";

    ptr += "<p>Water Level in upper tank: ";
    ptr += (float) data1;
    ptr += "cm</p>"; 
    ptr += "<p>Water level in lower tank: ";
    ptr +=(float) data2;
    ptr += "cm</p>";
    
    ptr += "</div>\n";
    ptr += "</body>\n";
    ptr += "</html>\n";

    return ptr;
    }
