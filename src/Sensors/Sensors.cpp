#include "Sensors.h"

 Sensors::Sensors() {
    this->dht = new DHT(SENSORS_DHT_PIN, SENSORS_DHT_TYPE);

    lastHumidity[0] = lastTemperature[0] = 0;
}

void Sensors::begin() {
    dht->begin();
}

char *Sensors::getCurrentHumidity() {
    return this->lastHumidity;
}

char *Sensors::getCurrentTemperature() {
    return this->lastTemperature;
}

void Sensors::calculateSensorsValues() {
    Serial.println("calculating temperature");
    this->calculateTemperature();
    Serial.println("calculating humidity");
    this->calculateHumidity();
}

void Sensors::calculateTemperature() {
    float temperature = dht->readTemperature();

    if (isnan(temperature)) {
        strncpy(lastTemperature, UNDEFINED_VALUE_TEXT, MAX_SENSORS_LENGTH);
        return;
    }

    int val_int = (int) temperature; // compute the integer part of the float
    float val_float = (abs(temperature) - abs(val_int))*100000;
    int val_fra = (int)val_float;
    sprintf (lastTemperature, "%d.%05d", val_int, val_fra); //
}

void Sensors::calculateHumidity() {
    float humidity = dht->readHumidity();

    if (isnan(humidity)) {
        strncpy(lastHumidity, UNDEFINED_VALUE_TEXT, MAX_SENSORS_LENGTH);
        return;
    }

    int val_int = (int) humidity; // compute the integer part of the float
    float val_float = (abs(humidity) - abs(val_int))*100000;
    int val_fra = (int)val_float;
    sprintf (lastHumidity, "%d.%05d", val_int, val_fra); //
}

