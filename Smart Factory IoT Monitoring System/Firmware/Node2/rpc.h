#ifndef RPC_H
#define RPC_H

#include <Arduino.h>
#include <PubSubClient.h>

// Init
void rpc_init(PubSubClient* client, char* buf, uint8_t size);

// MQTT callback
void rpc_mqttCallback(char* topic, byte* payload, unsigned int length);

#endif