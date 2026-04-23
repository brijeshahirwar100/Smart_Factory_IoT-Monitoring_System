#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

#include "config.h"
#include "SensorManager.h"
#include "network.h"
#include "telemetry.h"
#include "actuator.h"
#include "rpc.h"

// ─────────────────────────────────────────────
// GLOBAL OBJECTS
// ─────────────────────────────────────────────
EthernetClient eth;
PubSubClient mqtt(eth);

SensorManager sm;
SensorData data;

// Optimized buffers (UNO safe)
static char telemetryBuf[160];
static char rpcBuf[96];

unsigned long lastSend = 0;

// ─────────────────────────────────────────────
// SETUP
// ─────────────────────────────────────────────
void setup()
{
    Serial.begin(9600);
    Serial.println(F("\n[BOOT] Node1 Starting..."));

    // Initialize sensors
    SensorMgr_init(&sm);
    SensorMgr_begin();

    // Initialize actuators (relay + LEDs)
    actuators_begin();

    // Initialize telemetry buffer
    telemetry_init(telemetryBuf, sizeof(telemetryBuf));

    // Initialize network (Ethernet + MQTT)
    network_begin(&mqtt, NULL);

    // Initialize RPC handler
    rpc_init(&mqtt, rpcBuf, sizeof(rpcBuf));

    Serial.println(F("[BOOT] Initialization Complete"));
}

// ─────────────────────────────────────────────
// LOOP
// ─────────────────────────────────────────────
void loop()
{
    // Maintain MQTT + Ethernet connection
    network_maintain();

    unsigned long now = millis();

    // Send telemetry periodically
    if (now - lastSend > TELEMETRY_INTERVAL)
    {
        lastSend = now;

        Serial.println(F("\n[LOOP] Sampling + Sending..."));

        // Read sensors
        if (!SensorMgr_read(&sm, &data)) {
            Serial.println(F("[SENS] Read Failed ❌"));
        }

        // Send telemetry to ThingsBoard
        telemetry_publishTelemetry(&data, actuators_getRelayState());

        // Update LED status
        actuators_updateLED(
            network_isConnected(),
            data.sensorError,
            data.temperature
        );
    }
}