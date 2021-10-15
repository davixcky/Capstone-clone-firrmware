#ifndef SENSORS__H
#define SENSORS__H

#include <Arduino.h>
#include <DHT.h>

#define DHTTYPE DHT11

class Sensors {
    public:
        explicit Sensors(int dhtPin);

        void begin();

        String getCurrentTemperature();
        String getCurrentHumidity();
        void calculateSensorsValues();

    private:
        String lastTemperature;
        String lastHumidity;

        DHT *dht = nullptr;

        void calculateTemperature();
        void calculateHumidity();
};

#endif // SENSORS__H
