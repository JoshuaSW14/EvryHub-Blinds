#include <Arduino.h>
#include "secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "Wifi.h"
#include <Stepper.h>

#define AWS_IOT_PUBLISH_TOPIC   "evryhub/smartblinds"
#define AWS_IOT_SUBSCRIBE_TOPIC "evryhub/smartblinds"

#define PIN_RED    23 // GIOP23
#define PIN_GREEN  22 // GIOP22
#define PIN_BLUE   21 // GIOP21

const int stepsPerRevolution = 200;
Stepper myStepper(stepsPerRevolution, 32, 33, 34, 35);

int ldrValue;
const int ldrPin = 36; // or A0
const int ldrResolution = 12; // Could be 9-12

WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

void messageHandler(char* topic, byte* payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);

  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);

  const char* device = doc["device"];
  const char* action = doc["action"];

  if(String(device) == "led" && String(action) == "on"){
    Serial.println("Turn on");
    //digitalWrite(LED, HIGH);  
  }else if(String(device) == "led" && String(action) == "off"){
    Serial.println("Turn off");
    //digitalWrite(LED, LOW);
  }
}

void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.println("Connecting to Wi-Fi");
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  analogWrite(PIN_RED,   0);
  analogWrite(PIN_GREEN, 255);
  analogWrite(PIN_BLUE,  0);

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
 
  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);
 
  // Create a message handler
  client.setCallback(messageHandler);
 
  Serial.println("Connecting to AWS IOT");
 
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(100);
  }
 
  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }
 
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
 
  Serial.println("AWS IoT Connected!");
  analogWrite(PIN_RED,   0);
  analogWrite(PIN_GREEN, 0);
  analogWrite(PIN_BLUE,  255);
}
 
void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["device"] = "blinds";
  doc["action"] = "temperature:";
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void setup() {
  myStepper.setSpeed(60);
  Serial.begin(115200);

  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);

  analogWrite(PIN_RED,   255);
  analogWrite(PIN_GREEN, 0);
  analogWrite(PIN_BLUE,  0);

  connectAWS();
}

void loop() {

  

  //analogReadResolution(ldrResolution);
  //ldrValue = analogRead(ldrPin);
  ldrValue = digitalRead(ldrPin);
  Serial.println(ldrValue);

	// Serial.println("clockwise");
	// myStepper.step(stepsPerRevolution);
	// delay(500);

	// Serial.println("counterclockwise");
	// myStepper.step(-stepsPerRevolution);
	// delay(500);

  // publishMessage();
  // client.loop();
  delay(2000);
}