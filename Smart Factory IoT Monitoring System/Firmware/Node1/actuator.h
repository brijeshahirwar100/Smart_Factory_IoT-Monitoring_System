#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <Arduino.h>

void actuators_begin();
void actuators_setRelay(uint8_t state);
uint8_t actuators_getRelayState();
void actuators_updateLED(uint8_t mqtt, uint8_t error, float temp);

#endif