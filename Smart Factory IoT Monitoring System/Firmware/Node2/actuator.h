#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <Arduino.h>
#include "config.h"

// Initialize pins
void    actuators_begin(void);

// Relay control
void    actuators_setRelay(uint8_t on);
uint8_t actuators_getRelayState(void);

// Status LEDs - Ensure it takes exactly 4 arguments
void actuators_updateStatusLEDs(uint8_t mqttConnected,
                               uint8_t sensorError,
                               float temp,
                               float humidity,
                               uint8_t isDark);

#endif