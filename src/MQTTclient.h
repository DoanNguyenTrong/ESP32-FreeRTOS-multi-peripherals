#ifndef MQTTclient_h
#define MQTTclient_h

#include <PubSubClient.h>

class MQTTclient{
private:
    const char * client_name_;
    const char * topic_name_;
    const char * mqtt_server_;

public: 
    PubSubClient * client_;
    // Constructor and destructor
    MQTTclient();
    ~MQTTclient();
    MQTTclient & initiate(const char *, const char *, const char *, MQTT_CALLBACK_SIGNATURE, int port=1883);

    void reconnect();
    bool connected();
    bool loop();
};

#endif // MQTTclient_h