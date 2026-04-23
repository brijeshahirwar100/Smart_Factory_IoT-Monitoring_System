#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include "config.h"

// ─────────────────────────────────────────────
// NETWORK MODULE API
// ─────────────────────────────────────────────

// Initialize Ethernet + MQTT
// mqtt → pointer to PubSubClient
// onConnected → callback when MQTT connects
void    network_begin(PubSubClient* mqtt, void (*onConnected)(void));

// Maintain connection (call inside loop)
void    network_maintain(void);

// Check MQTT connection status
uint8_t network_isConnected(void);

// Publish data to MQTT
uint8_t network_publish(const char* topic, const char* payload, uint8_t retain);

// Subscribe to topic (optional future use)
uint8_t network_subscribe(const char* topic);

#endif // NETWORK_H