#include "rpc.h"
#include "actuator.h"
#include "SensorManager.h"
#include <avr/wdt.h>
#include "telemetry.h"
// ─────────────────────────────────────────────
// GLOBALS
// ─────────────────────────────────────────────
static PubSubClient* rpcMqtt  = NULL;
static char*         rpcBuf   = NULL;
static uint8_t       rpcBufLen = 0;

// ─────────────────────────────────────────────
// EXTRACT REQUEST ID
// ─────────────────────────────────────────────
static void extractRequestId(const char* topic, char out[RPC_ID_SIZE]) {
    const char* slash = strrchr(topic, '/');
    if (slash) {
        strncpy(out, slash + 1, RPC_ID_SIZE - 1);
    } else {
        out[0] = '\0';
    }
    out[RPC_ID_SIZE - 1] = '\0';
}

// ─────────────────────────────────────────────
// SEND RESPONSE
// ─────────────────────────────────────────────
static void sendResponse(const char* reqId, const char* json) {
    char topic[64];
    snprintf_P(topic, sizeof(topic),
        PSTR("%s%s"), TOPIC_RPC_RESPONSE, reqId);

    rpcMqtt->publish(topic, json);
}

// ─────────────────────────────────────────────
// SET RELAY
// ─────────────────────────────────────────────
static void handleSetRelay(const char* reqId, const char* params)
{
    uint8_t on = (params && strstr(params, "true")) ? 1 : 0;

    actuators_setRelay(on);

    Serial.print(F("[RPC] Relay set to: "));
    Serial.println(on ? F("ON") : F("OFF"));

    // 🔥 IMMEDIATE TELEMETRY UPDATE (VERY IMPORTANT)
    extern SensorData data;   // from main
    telemetry_publishTelemetry(&data, on);

    // Response
    snprintf_P(rpcBuf, rpcBufLen,
        PSTR("{\"success\":true,\"relay\":%u}"), on);

    sendResponse(reqId, rpcBuf);
}
// ─────────────────────────────────────────────
// GET STATUS
// ─────────────────────────────────────────────
static void handleGetStatus(const char* reqId) {

    snprintf_P(rpcBuf, rpcBufLen,
        PSTR("{\"success\":true,\"relay\":%u,\"uptime\":%lu}"),
        (unsigned)actuators_getRelayState(),
        millis() / 1000UL
    );

    Serial.println(F("[RPC] Status requested"));

    sendResponse(reqId, rpcBuf);
}

// ─────────────────────────────────────────────
// UNKNOWN
// ─────────────────────────────────────────────
static void handleUnknown(const char* reqId) {
    snprintf_P(rpcBuf, rpcBufLen,
        PSTR("{\"success\":false,\"code\":404}"));

    sendResponse(reqId, rpcBuf);
}

// ─────────────────────────────────────────────
// INIT
// ─────────────────────────────────────────────
void rpc_init(PubSubClient* mqtt, char* buf, uint8_t bufLen) {
    rpcMqtt   = mqtt;
    rpcBuf    = buf;
    rpcBufLen = bufLen;
}

// ─────────────────────────────────────────────
// MQTT CALLBACK
// ─────────────────────────────────────────────
void rpc_mqttCallback(char* topic, byte* payload, unsigned int length) {

    const char* mStart;
    const char* params;
    char reqId[RPC_ID_SIZE];

    if (!rpcMqtt) return;

    // Copy payload safely
    if (length >= rpcBufLen) length = rpcBufLen - 1;
    memcpy(rpcBuf, payload, length);
    rpcBuf[length] = '\0';

    Serial.println(F("\n[RPC] Received:"));
    Serial.println(rpcBuf);

    extractRequestId(topic, reqId);

    // Find method
    mStart = strstr(rpcBuf, "\"method\":\"");
    if (!mStart) { handleUnknown(reqId); return; }
    mStart += 10;

    if (!strchr(mStart, '"')) { handleUnknown(reqId); return; }

    // Find params
    params = strstr(rpcBuf, "\"params\":");
    if (params) params += 9;

    // Dispatch
    if      (strncmp_P(mStart, PSTR("setRelay"),  8) == 0)
        handleSetRelay(reqId, params);

    else if (strncmp_P(mStart, PSTR("getStatus"), 9) == 0)
        handleGetStatus(reqId);

    else
        handleUnknown(reqId);
}