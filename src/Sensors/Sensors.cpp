#include "Sensors.h"

 Sensors::Sensors(int dhtPin) {
    this->dht = new DHT(dhtPin, DHTTYPE);
}

void Sensors::begin() {
    dht->begin();
}

String Sensors::getCurrentHumidity() {
    return this->lastHumidity;
}

String Sensors::getCurrentTemperature() {
    return this->lastTemperature;
}

void Sensors::calculateSensorsValues() {
    this->calculateTemperature();
    this->calculateHumidity();
}

void Sensors::calculateTemperature() {
    float temperature = dht->readTemperature();

    this->lastTemperature = isnan(temperature) ? "n/a" : String(temperature);
}

void Sensors::calculateHumidity() {
    float humidity = dht->readHumidity();

    this->lastHumidity = isnan(humidity) ? "n/a" : String(humidity);
}

