#include "telemetry.h"
#include "network.h"
#include <stdio.h>

// ─────────────────────────────────────────────
// GLOBAL BUFFER POINTER
// ─────────────────────────────────────────────
static char* buf = NULL;
static uint8_t len = 0;

// ─────────────────────────────────────────────
// INIT
// ─────────────────────────────────────────────
void telemetry_init(char* b, uint8_t l)
{
    buf = b;
    len = l;
}

// ─────────────────────────────────────────────
// TELEMETRY SEND
// ─────────────────────────────────────────────
void telemetry_publishTelemetry(const SensorData* d, uint8_t relay)
{
    if (buf == NULL || len == 0) {
        Serial.println(F("[TEL] ERROR: Buffer not initialized!"));
        return;
    }

    // ─────────────────────────────────────────
    // FLOAT → STRING CONVERSION (CRITICAL FIX)
    // ─────────────────────────────────────────
    char tStr[10], hStr[10], lmStr[10];

    dtostrf(d->temperature, 4, 2, tStr);
    dtostrf(d->humidity, 4, 2, hStr);
    dtostrf(d->lm35Temp, 4, 2, lmStr);

    // ─────────────────────────────────────────
    // BUILD JSON
    // ─────────────────────────────────────────
    int n = snprintf_P(buf, len,
        PSTR("{"
             "\"temperature\":%s,"
             "\"humidity\":%s,"
             "\"lm35Temp\":%s,"
             "\"vibration\":%u,"
             "\"machineStatus\":%u,"
             "\"relayState\":%u,"
             "\"sensorError\":%u,"
             "\"uptime\":%lu"
             "}"),
        tStr,
        hStr,
        lmStr,
        d->vibration,
        d->machineStatus,
        relay,
        d->sensorError,
        millis()/1000UL
    );

    if (n <= 0) {
        Serial.println(F("[TEL] ERROR: snprintf failed!"));
        return;
    }

    // ─────────────────────────────────────────
    // DEBUG PRINT
    // ─────────────────────────────────────────
    Serial.println(F("\n-------------------------------"));
    Serial.println(F("[TEL] Sending Data:"));

    Serial.print(F("Temp (DHT): "));
    Serial.print(d->temperature);
    Serial.println(F(" °C"));

    Serial.print(F("Humidity  : "));
    Serial.print(d->humidity);
    Serial.println(F(" %"));

    Serial.print(F("LM35 Temp : "));
    Serial.print(d->lm35Temp);
    Serial.println(F(" °C"));

    Serial.print(F("Vibration : "));
    Serial.println(d->vibration);

    Serial.print(F("Machine   : "));
    Serial.println(d->machineStatus ? F("ON") : F("OFF"));

    Serial.print(F("Relay     : "));
    Serial.println(relay ? F("ON") : F("OFF"));

    Serial.print(F("Error     : "));
    Serial.println(d->sensorError ? F("YES") : F("NO"));

    Serial.print(F("Uptime    : "));
    Serial.print(millis()/1000UL);
    Serial.println(F(" sec"));

    Serial.println(F("-------------------------------"));

    // JSON print
    Serial.println(F("[TEL] JSON:"));
    Serial.println(buf);

    // ─────────────────────────────────────────
    // MQTT SEND
    // ─────────────────────────────────────────
    if (network_isConnected())
    {
        Serial.println(F("[TEL] Publishing..."));
        network_publish(TOPIC_TELEMETRY, buf, 0);
    }
    else
    {
        Serial.println(F("[TEL] MQTT NOT CONNECTED ❌"));
    }
}