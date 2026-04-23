#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include "config.h"

// Define the struct for sensor data
typedef struct {
    float    temperature;
    float    humidity;
    uint16_t ldrValue;
    bool     motionDetected;
    bool     doorOpen;
    bool     isDark;
    bool     sensorError;
} SensorData;

// Define the struct for the manager context
typedef struct {
    float         lastTemp;
    float         lastHumidity;
    bool          motionActive;
    unsigned long motionClearTime;
} SensorManager;

// Function Prototypes
void SensorMgr_init  (SensorManager* sm);
void SensorMgr_begin (SensorManager* sm);
bool SensorMgr_read  (SensorManager* sm, SensorData* out);

#endif