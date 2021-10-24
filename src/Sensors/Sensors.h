#ifndef SENSORS__H
#define SENSORS__H

#include "globals.h"

#include <Arduino.h>
#include <DHT.h>

class Sensors {
public:
    Sensors();

    static Sensors &Instance() {
        static Sensors instance;
        return instance;
    }

    void begin();
    char *getCurrentTemperature();
    char *getCurrentHumidity();
    void calculateSensorsValues();

private:
    char lastTemperature[MAX_SENSORS_LENGTH];
    char lastHumidity[MAX_SENSORS_LENGTH];

    DHT *dht = nullptr;

    void calculateTemperature();
    void calculateHumidity();
};

#endif // SENSORS__H
