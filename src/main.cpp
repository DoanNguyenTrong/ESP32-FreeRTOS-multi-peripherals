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


const char * ssid = "fddb6a";
const char * password = "bride.018.unity";
const char* mqtt_server = "192.168.0.6";

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;
// LED Pin
const int ledPin = 13;

void reconnect();
void callback(char * , byte *, unsigned int);
void setup_wifi();


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



void setup()
{
  Serial.begin(112500);
  pinMode(ledPin, OUTPUT);
  xTaskCreate( anotherTask,  "another Task", 10000, NULL,1,NULL); /* Task handle to keep track of created task */
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
 

/* the forever loop() function is invoked by Arduino ESP32 loopTask */
void loop()
{
  // Serial.println("Main ESP32 Loop!!");
  // delay(1000);
  if (!client.connected()){
    reconnect();
  }
  client.loop();

  // long now = millis();
  // delay(5000);
}




void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
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
