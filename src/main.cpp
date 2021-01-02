#include <Arduino.h>
#include <FreeRTOS.h>
#include <WiFi.h>

// User included libraries
#include <ESP_FlexyStepper.h> // Stepper
#include <ESP32_Servo.h>      // Servo
#include <PubSubClient.h>     // MQTT
#include <MFRC522.h>          // RFID
#include <Adafruit_Sensor.h>  // Dependency of DHT sensor
#include <DHT.h>              // DHT sensor
#include <LiquidCrystal.h>    // LCD

// User custom code
#include "MQTTclient.h"
#include "MultiTasks.h"


void setup_wifi();
void anotherTask( void *parameter);

// LED Pin
const int ledPin = 13;

const char * ssid = "fddb6a";
const char * password = "bride.018.unity";

const char* mqtt_server = "192.168.0.6";

long lastMsg = 0;
char msg[50];
int value = 0;

MQTTclient mqtt;

void setup()
{
  Serial.begin(112500);
  pinMode(ledPin, OUTPUT);
  xTaskCreate( anotherTask,  "another Task", 10000, NULL,1,NULL); /* Task handle to keep track of created task */
  
  setup_wifi();

  mqtt.initiate("ESP_1", "esp32/output", mqtt_server, callback);
}


/* the forever loop() function is invoked by Arduino ESP32 loopTask */
void loop()
{
  // Serial.println("Main ESP32 Loop!!");
  // delay(1000);
  if (!mqtt.connected()){
    mqtt.reconnect();
  }
  mqtt.loop();

  // long now = millis();
  // delay(5000);
}


void callback(char * topic, byte *message, unsigned int length){
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  // Changes the output state according to the message
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }
  }
}


void setup_wifi(){
  delay(10);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWifi connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}



/* this function will be invoked when additionalTask was created */
void anotherTask( void * parameter ){
  for(;;){
    Serial.println("Control LED in anotherTask!");
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
    delay(1000);

  }

  vTaskDelete( NULL );
}

