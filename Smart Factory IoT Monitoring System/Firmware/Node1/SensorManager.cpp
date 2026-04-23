#include "SensorManager.h"
#include "config.h"
#include <DHT.h>

static DHT dht(PIN_DHT, DHT_TYPE);

void SensorMgr_init(SensorManager* sm)
{
    sm->lastTemp = 0;
    sm->lastHumidity = 0;
}

void SensorMgr_begin()
{
    pinMode(PIN_BUTTON, INPUT_PULLUP);
    dht.begin();
}

static float readLM35()
{
    int adc = analogRead(PIN_LM35);
    return (adc * 500.0) / 1023.0;
}

static uint16_t readVibration()
{
    uint32_t sum = 0;
    for (int i = 0; i < 4; i++) {
        sum += analogRead(PIN_VIB);
    }
    return sum / 4;
}

static bool readMachine()
{
    return digitalRead(PIN_BUTTON) == LOW;
}

bool SensorMgr_read(SensorManager* sm, SensorData* out)
{
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (isnan(t) || isnan(h)) {
        out->sensorError = true;
        out->temperature = sm->lastTemp;
        out->humidity = sm->lastHumidity;
    } else {
        out->sensorError = false;
        out->temperature = t;
        out->humidity = h;

        sm->lastTemp = t;
        sm->lastHumidity = h;
    }

    out->lm35Temp = readLM35();
    out->vibration = readVibration();
    out->machineStatus = readMachine();

    return true;
}