#include "debug.h"

void debug_printSensors(const SensorData* d)
{
#if DEBUG_ENABLE
    Serial.print("[SENSOR] T=");
    Serial.print(d->temperature);
    Serial.print("C H=");
    Serial.print(d->humidity);
    Serial.print("% LDR=");
    Serial.print(d->ldrValue);
    Serial.print(" Dark=");
    Serial.print(d->isDark);
    Serial.print(" Motion=");
    Serial.print(d->motionDetected);
    Serial.print(" Door=");
    Serial.print(d->doorOpen);
    Serial.print(" Err=");
    Serial.println(d->sensorError);
#endif
}

void debug_printActuator(uint8_t relay)
{
#if DEBUG_ENABLE
    Serial.print("[ACT] Relay=");
    Serial.println(relay ? "ON" : "OFF");
#endif
}

void debug_printNetwork(uint8_t connected)
{
#if DEBUG_ENABLE
    Serial.print("[NET] MQTT=");
    Serial.println(connected ? "OK" : "DOWN");
#endif
}

void debug_printTelemetry(const char* json)
{
#if DEBUG_ENABLE
    Serial.println("[TELEMETRY]");
    Serial.println(json);
#endif
}