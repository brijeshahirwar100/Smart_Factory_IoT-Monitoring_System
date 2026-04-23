#include "actuator.h"
#include "config.h"

static uint8_t relayState = 0;

void actuators_begin()
{
    pinMode(PIN_RELAY, OUTPUT);
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_YELLOW, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);
}

void actuators_setRelay(uint8_t state)
{
    relayState = state;
    digitalWrite(PIN_RELAY, state);
}

uint8_t actuators_getRelayState()
{
    return relayState;
}

void actuators_updateLED(uint8_t mqtt, uint8_t error, float temp)
{
    // Turn OFF all first (VERY IMPORTANT)
    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_YELLOW, LOW);
    digitalWrite(PIN_LED_RED, LOW);

    if (!mqtt)
    {
        digitalWrite(PIN_LED_RED, HIGH);   // Not connected
        return;
    }

    if (error)
    {
        digitalWrite(PIN_LED_YELLOW, HIGH); // Sensor error
        return;
    }

    if (temp > 35)
    {
        digitalWrite(PIN_LED_RED, HIGH);   // Critical
        return;
    }

    // Normal
    digitalWrite(PIN_LED_GREEN, HIGH);
}