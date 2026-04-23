#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <Arduino.h>
#include "config.h"
#include "SensorManager.h"

void telemetry_init(char* buf, uint8_t len);
void telemetry_publishTelemetry(const SensorData* data, uint8_t relayState);

#endif