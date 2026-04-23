#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>

typedef struct {
    float temperature;
    float humidity;
    float lm35Temp;
    uint16_t vibration;
    bool machineStatus;
    bool sensorError;
} SensorData;

typedef struct {
    float lastTemp;
    float lastHumidity;
} SensorManager;

void SensorMgr_init(SensorManager* sm);
void SensorMgr_begin();
bool SensorMgr_read(SensorManager* sm, SensorData* data);

#endif