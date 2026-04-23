#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>
#include "SensorManager.h"

// Enable / Disable globally
#define DEBUG_ENABLE 1

void debug_printSensors(const SensorData* d);
void debug_printActuator(uint8_t relay);
void debug_printNetwork(uint8_t connected);
void debug_printTelemetry(const char* json);

#endif