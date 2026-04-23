#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

#include "config.h"
#include "SensorManager.h"
#include "network.h"
#include "telemetry.h"
#include "actuator.h"
#include "rpc.h"
#include "debug.h"

EthernetClient ethClient;
PubSubClient mqttClient(ethClient);

SensorManager sm;
SensorData data;

uint8_t manualOverride = 0;
char rpcBuf[96];

unsigned long lastSend = 0;

// ✅ REQUIRED callback
void onMqttConnected() {
    Serial.println("[MQTT] Connected Callback");
}

void setup()
{
    Serial.begin(9600);

    SensorMgr_init(&sm);
    SensorMgr_begin(&sm);
    actuators_begin();

    // ✅ FIXED: now matches signature
    network_begin(&mqttClient, onMqttConnected);

    // ✅ FIXED: correct arguments
    rpc_init(&mqttClient, rpcBuf, sizeof(rpcBuf));
}

void loop()
{
    network_maintain();

    if (millis() - lastSend > TELEMETRY_INTERVAL)
    {
        lastSend = millis();

        SensorMgr_read(&sm, &data);
        debug_printSensors(&data);
        debug_printNetwork(network_isConnected());

        // 🔥 AUTO CONTROL WITH OVERRIDE
        if (!manualOverride)
        {
            if (data.humidity >= HUMIDITY_CRIT)
                actuators_setRelay(1);
            else if (data.humidity <= HUMIDITY_HYST)
                actuators_setRelay(0);
        }

        telemetry_publishTelemetry(&data, actuators_getRelayState());

        actuators_updateStatusLEDs(
            network_isConnected(),
            data.sensorError,
            data.temperature,
            data.humidity,
            data.isDark
        );
    }
}