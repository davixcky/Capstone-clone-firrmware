#ifndef GLOBALS__H
#define GLOBALS__H

#include <Arduino.h>

// Credentials for Captive Portal
#define AP_SSID "UNINORTE_EXPOFISICA"
// We are not using currently the password, so it can be removed
#define AP_PASSWORD "uninorteadmin"
// TODO: Flag to use or not password

// General PATH
#define CONFIGURATION_PATH "/config.def"
#define MAX_CHARACTERS_CONF_LINE 30

#define MAX_SSID_LENGTH 30
#define MAX_PASSWORD_LENGTH 30
#define MAX_DEVICE_NAME_LENGTH 20

// Sensors
#define SENSORS_DHT_PIN 32
#define SENSORS_DHT_TYPE DHT11

// Data controller
#define MAX_BUCKET_ID_LENGTH 20
#define MAX_TOKEN_BUCKET_LENGTH 20
#define MAX_SENSORS_LENGTH 30

static const char *UNDEFINED_VALUE_TEXT =  "n/a";
static const char *RUNNING_TEXT = "Running";
static const char *END_TEXT = "Waiting";

#endif // GLOBALS__H
