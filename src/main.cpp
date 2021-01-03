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



#include <atomic>
#include <thread>
#include <mutex>

// User custom code
#include "MQTTclient.h"
#include "MultiTasks.h"


void setup_wifi();
void servoThread( void *parameter);
void callback(char * , byte *, unsigned int );

// 1 channel Relay
const int SS_Pin   = 15;

// Servo motor
const int servoPin = 13;  // Servo

// Step motor
const int stepPin1 = 12;  // Stepper INT1
const int stepPin2 = 14;  // Stepper INT2
const int stepPin3 = 27;  // Stepper INT3
const int stepPin4 = 26;  // Stepper INT4

// LCD: The ESP32 has two I2C channels
const int lcdSDA   = 21;  // LCD SDA
const int lcdSCL   = 22;  // LCD SCL

// Motion sensor
const int motionPin= 32;  // PIR motion sensor
const int dhtPin   = 35;  // DHT temp + rh pin
// RFRID
const int RST_Pin  = 34;  // RFID Reset pin
const int SS_Pin   = 39;  // RFID Slave select pin

// Keypad 4x4
// https://diyi0t.com/keypad-arduino-esp8266-esp32/
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
byte rowPins[ROWS] = {16, 4, 0, 2}; 
byte colPins[COLS] = {19, 18, 5, 17};


const char * ssid = "fddb6a";
const char * password = "bride.018.unity";
const char* mqtt_server = "192.168.0.6";


long lastMsg = 0;
char msg[50];
int value = 0;

// Initiate all objects

MQTTclient mqtt;


void setup()
{
  Serial.begin(112500);
  pinMode(ledPin, OUTPUT);


  xTaskCreate( servoThread,  "another Task", 10000, NULL,1,NULL); /* Task handle to keep track of created task */
  
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