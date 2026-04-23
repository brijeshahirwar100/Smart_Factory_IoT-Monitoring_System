#include "telemetry.h"
#include "network.h"
#include "debug.h"

void telemetry_publishTelemetry(const SensorData* d, uint8_t relay)
{
  char buffer[300];

  char tStr[10], hStr[10];

  dtostrf(d->temperature, 4, 1, tStr);
  dtostrf(d->humidity, 4, 1, hStr);

snprintf(buffer, sizeof(buffer),
  "{\"temperature\":%s,\"humidity\":%s,"
  "\"ldrValue\":%u,\"isDark\":%u,"
  "\"motionDetected\":%u,\"doorOpen\":%u,"
  "\"relayState\":%u,\"sensorError\":%u,"
  "\"uptime\":%lu}",
  tStr,
  hStr,
  d->ldrValue,
  d->isDark,
  d->motionDetected,
  d->doorOpen,
  relay,
  d->sensorError,
  millis()/1000UL
);

  // 🔥 THIS LINE WAS MISSING
  network_publish(TOPIC_TELEMETRY, buffer, 0);

  // (optional debug)
  debug_printTelemetry(buffer);
}