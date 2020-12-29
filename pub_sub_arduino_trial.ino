/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "Lenovo C2";
const char* password = "507522d2fe1a";
const char* mqtt_server = "35.192.49.64";

WiFiClient espClient;
PubSubClient client(espClient);
String arrived = "";
float h = 0.00;
String Motorstate = "off";
char msg1[5];
char msg2[5];

//outtopic = "Watersense/test/out/device1";
//intopic = "Watersense/test/in/device1";
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
//192.168.43.12

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println();
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    arrived += (char)payload[i];
  }
  Serial.println();
  Serial.println(arrived);
  client.publish("Watersense/test/out/device1", "Message received!");
 h = arrived.toFloat();
 Serial.print("New value of h = ");
 Serial.println(h);
 }

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
       dtostrf(h, 4, 2, msg1);
    Motorstate.toCharArray(msg2, 5);
    Serial.print("Publish message: ");
    Serial.print(h);
    Serial.println(Motorstate);
    client.publish("Watersense/test/out/device1", msg1);
    client.publish("Watersense/test/out/device1", msg2);
      // ... and resubscribe
      client.subscribe("Watersense/test/in/device1");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);//calling callback
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
    dtostrf(h, 4, 2, msg1);
    Motorstate.toCharArray(msg2, 5);
    Serial.print("Publish message: ");
    Serial.print(h);
    Serial.println(Motorstate);
    client.publish("Watersense/test/out/device1", msg1);
    client.publish("Watersense/test/out/device1", msg2);
    delay(5000);
}
