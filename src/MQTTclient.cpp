#include <Arduino.h>
#include <FreeRTOS.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "MQTTclient.h"

MQTTclient::MQTTclient(){
  this->client_name_ = nullptr;
  this->topic_name_ = nullptr;
  this->mqtt_server_ = nullptr;
  this->client_ = nullptr;
}

MQTTclient::~MQTTclient(){

}


MQTTclient & MQTTclient::initiate(const char *client_name, const char *topic_name, const char *mqtt_server, 
                                  MQTT_CALLBACK_SIGNATURE, int port){
  this->topic_name_ = topic_name;
  this->client_name_ = client_name;
  WiFiClient espClient;
  PubSubClient client(espClient);
  this->client_ = &client;
  this->client_->setServer(this->mqtt_server_, port);
  this->client_->setCallback(callback);
  return *this;
}

bool MQTTclient::connected(){
  return this->client_->connected();
}

bool MQTTclient::loop(){
  return this->client_->loop();
}


void MQTTclient::reconnect() {
  // Loop until we're reconnected
  while (!client_->connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if ((*this->client_).connect(this->client_name_)) {
      Serial.println("connected");
      // Subscribe
      client_->subscribe(this->topic_name_);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client_->state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}