#include "SensorManager.h"
#include <DHT.h>

static DHT dht(PIN_DHT, DHT_TYPE);

void SensorMgr_init(SensorManager* sm)
{
  sm->lastTemp = 0;
  sm->lastHumidity = 0;
  sm->motionActive = false;
  sm->motionClearTime = 0;
}

void SensorMgr_begin(SensorManager* sm)
{
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_DOOR, INPUT_PULLUP);
  dht.begin();
}

// PIR (10s hold)
static bool readPIR(SensorManager* sm)
{
  bool pir = digitalRead(PIN_PIR);
  unsigned long now = millis();

  if (pir) {
    sm->motionActive = true;
    sm->motionClearTime = now + MOTION_CLEAR_MS;
  }
  else if (sm->motionActive && now > sm->motionClearTime) {
    sm->motionActive = false;
  }

  return sm->motionActive;
}

// Door
static bool readDoor()
{
  return digitalRead(PIN_DOOR) == LOW;
}

// LDR avg
static uint16_t readLDR()
{
  uint32_t sum = 0;

  for (int i = 0; i < LDR_SAMPLES; i++) {
    sum += analogRead(PIN_LDR);
    delay(LDR_SAMPLE_DLY);
  }

  return sum / LDR_SAMPLES;
}

bool SensorMgr_read(SensorManager* sm, SensorData* out)
{
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  out->ldrValue = readLDR();
  out->isDark = (out->ldrValue < LDR_DARK_THRESH);
  out->motionDetected = readPIR(sm);
  out->doorOpen = readDoor();

  if (isnan(t) || isnan(h)) {
    out->sensorError = true;
    out->temperature = sm->lastTemp;
    out->humidity = sm->lastHumidity;
    return false;
  }

  out->sensorError = false;
  out->temperature = t;
  out->humidity = h;

  sm->lastTemp = t;
  sm->lastHumidity = h;

  return true;
}
