#ifndef STORAGE__H
#define STORAGE__H

#include "globals.h"

#include "Utils/Strings.h"

#include <Arduino.h>
#include <FS.h>
#include <SD.h>

class Storage {
public:
    static void init();

    void saveWifiCredentials();
    void setNewCredentials(const char *ssid, const char *password);
    void printCurrentCredentials();

    boolean containsCredentials();

    static Storage &Instance() {
        static Storage instance;
        return instance;
    }

private:
    boolean _isWifiSet = false;
    char _wifi_ssid[MAX_CHARACTERS_CONF_LINE];
    char _wifi_password[MAX_CHARACTERS_CONF_LINE];
    uint8_t _wifi_ssid_length = 0;
    uint8_t _wifi_password_length = 0;

    void loadConfig();
};


#endif //STORAGE__H
