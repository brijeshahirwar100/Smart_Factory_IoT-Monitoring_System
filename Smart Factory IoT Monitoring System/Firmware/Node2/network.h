#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include "config.h"

// Init (WITH callback)
void    network_begin(PubSubClient* mqtt, void (*onConnected)(void));

// Maintain connection
void    network_maintain(void);

// Status
uint8_t network_isConnected(void);

// Publish (WITH retain)
uint8_t network_publish(const char* topic, const char* payload, uint8_t retain);

// Subscribe
uint8_t network_subscribe(const char* topic);

#endif