#include "actuator.h"

static uint8_t relayState = 0;

void actuators_begin(void)
{
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_YELLOW, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);

  digitalWrite(PIN_RELAY, LOW);
}

void actuators_setRelay(uint8_t on)
{
  relayState = on;
  digitalWrite(PIN_RELAY, on ? HIGH : LOW);
  
}

uint8_t actuators_getRelayState(void)
{
  return relayState;
}

// LED logic
void actuators_updateStatusLEDs(uint8_t mqttConnected,
                               uint8_t sensorError,
                               float temp,
                               float humidity,
                               uint8_t isDark)

{

  if (!mqttConnected) {
    digitalWrite(PIN_LED_RED, HIGH);
   // digitalWrite(PIN_LED_GREEN, LOW);
   // digitalWrite(PIN_LED_YELLOW, LOW);
    return;
  }

  if (sensorError) {
   // digitalWrite(PIN_LED_YELLOW, millis() % 500 < 250);
    digitalWrite(PIN_LED_YELLOW, HIGH);
    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_RED, LOW);
    return;
  }

  if (temp > 35 || humidity >= HUMIDITY_CRIT) {
    digitalWrite(PIN_LED_RED, HIGH);
    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_YELLOW, LOW);
  }
  else if (temp > 30 || humidity >= HUMIDITY_WARN || isDark) {
    digitalWrite(PIN_LED_YELLOW, HIGH);
    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_RED, LOW);
  }
  else {
    digitalWrite(PIN_LED_GREEN, HIGH);
    digitalWrite(PIN_LED_YELLOW, LOW);
    digitalWrite(PIN_LED_RED, LOW);
  }
}