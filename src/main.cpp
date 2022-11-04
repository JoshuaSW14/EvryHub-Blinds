#include <Arduino.h>
#include "secrets.h"
#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Stepper.h>

// AWS Pub/Sub Topics & Wifi Config
#define AWS_IOT_PUBLISH_TOPIC "evryhub/blinds"
#define AWS_IOT_SUBSCRIBE_TOPIC "evryhub/blinds/sub"
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

// LED Pin (digital)
#define LED_PIN 21

// LDR Pin (analog)
#define LDR_PIN 32

// Button Pins (digital)
#define upButtonPin 23
#define downButtonPin 22

// Stepper Configuration (digital)
int stepsPerRevolution = 500;
Stepper myStepper(stepsPerRevolution, 27, 25, 26, 33); // IN1, IN3, IN2, IN4

// Ambient Light Sensor
int ldrValue;
const int ldrResolution = 12; // Could be 9-12

void openBlinds()
{
  myStepper.step(stepsPerRevolution);
}

void closeBlinds()
{
  myStepper.step(-stepsPerRevolution);
}

void configBlinds(int direction)
{
  stepsPerRevolution = stepsPerRevolution + direction;
}

void messageHandler(char *topic, byte *payload, unsigned int length)
{
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);

  const char *device = doc["device"];
  const char *action = doc["action"];
  const char *value = doc["value"];

  Serial.printf("\n Device: %s | Action: %s | Value: %s\n", String(device), String(action), String(value));

  if (String(device) == "blinds" && String(action) == "config")
  {
    Serial.println("Configure EvryHub Blinds");
    configBlinds(atoi(value));
  }
  else if (String(device) == "blinds" && String(action) == "raise")
  {
    Serial.println("Open Blinds");
    openBlinds();
  }
  else if (String(device) == "blinds" && String(action) == "lower")
  {
    Serial.println("Close Blinds");
    closeBlinds();
  }
}

void connectAWS()
{
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
  digitalWrite(LED_PIN, LOW);

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  Serial.println("AWS IoT Connected!");
}

void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["device"] = "blinds";
  doc["action"] = "light";
  doc["value"]  = String(ldrValue);
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void otaSetup()
{
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    Serial.print(".");
  }
  digitalWrite(LED_PIN, HIGH);

  ArduinoOTA
      .onStart([]()
               {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type); })
      .onEnd([]()
             { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed"); });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void checkInput()
{
  while (digitalRead(upButtonPin) == LOW)
  {
    myStepper.step(stepsPerRevolution);
  }

  while (digitalRead(downButtonPin) == LOW)
  {
    myStepper.step(-stepsPerRevolution);
  }
}

void setup()
{
  myStepper.setSpeed(60);
  Serial.begin(115200);

  pinMode(upButtonPin, INPUT_PULLUP);
  pinMode(downButtonPin, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  otaSetup();
  connectAWS();
}

void loop()
{
  ArduinoOTA.handle();
  analogReadResolution(ldrResolution);
  ldrValue = analogRead(LDR_PIN);

  checkInput();
  publishMessage();
  client.loop();
}