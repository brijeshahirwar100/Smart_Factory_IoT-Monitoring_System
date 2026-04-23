#include <Arduino.h>
#ifndef CONFIG_H
#define CONFIG_H

// Network
#define NODE_MAC           { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }
#define TB_SERVER          "mqtt.thingsboard.cloud"
#define TB_PORT            1883
#define ACCESS_TOKEN       "QlcJuhZVHdfueYce7JZE"   // 🔥 replace
#define CLIENT_ID          "Warehouse_monitor_Node2"

// MQTT
#define TOPIC_TELEMETRY    "v1/devices/me/telemetry"
#define TOPIC_RPC_REQUEST  "v1/devices/me/rpc/request/+"

// Intervals
#define TELEMETRY_INTERVAL 5000UL
#define RECONNECT_INTERVAL 5000UL

// Pins
#define PIN_DHT        2
#define PIN_PIR        3
#define PIN_RELAY      4
#define PIN_LED_GREEN  5
#define PIN_LED_YELLOW 6
#define PIN_LED_RED    8
#define PIN_DOOR       7
#define PIN_LDR        A0

// Sensors
#define DHT_TYPE           DHT22
#define LDR_SAMPLES        4
#define LDR_SAMPLE_DLY     2
#define LDR_DARK_THRESH    300

// Thresholds
#define HUMIDITY_WARN      70.0f
#define HUMIDITY_CRIT      85.0f
#define HUMIDITY_HYST      80.0f   // 🔥 added hysteresis

#define MOTION_CLEAR_MS    10000UL

#endif
#define DEBUG_ENABLE 1
