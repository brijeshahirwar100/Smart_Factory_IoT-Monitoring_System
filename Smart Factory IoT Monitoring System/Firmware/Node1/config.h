#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define TOPIC_RPC_REQUEST  "v1/devices/me/rpc/request/+"
#define TOPIC_RPC_RESPONSE "v1/devices/me/rpc/response/"
#define RPC_ID_SIZE 16

// -------- Network --------
#define NODE_MAC { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 }

#define TB_SERVER "mqtt.thingsboard.cloud"
#define TB_PORT   1883
#define ACCESS_TOKEN "MUTMTwQ9f98VYB9QJ4kf"
#define CLIENT_ID "Node1_Production_Line"

// -------- MQTT --------
#define TOPIC_TELEMETRY "v1/devices/me/telemetry"
#define TOPIC_RPC_REQUEST  "v1/devices/me/rpc/request/+"
#define TOPIC_RPC_RESPONSE "v1/devices/me/rpc/response/"
#define RPC_ID_SIZE 16

// -------- Pins --------
#define PIN_DHT        2
#define PIN_BUTTON     3
#define PIN_RELAY      4
#define PIN_LED_GREEN  5
#define PIN_LED_YELLOW 6
#define PIN_LED_RED    7
#define PIN_VIB        A0
#define PIN_LM35       A1

// -------- Sensor --------
#define DHT_TYPE DHT22

// -------- Timing --------
#define TELEMETRY_INTERVAL 5000
#define RECONNECT_INTERVAL 5000

#endif