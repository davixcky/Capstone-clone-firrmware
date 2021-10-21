#ifndef SERVER__H
#define SERVER__H

#include "../globals.h"

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SD.h>
#include <WiFi.h>
#include <DNSServer.h>

#include "CaptiveRequestHandler.h"
#include <Storage/Storage.h>
#include <Display/Display.h>


class ServerManager {
public:
    ServerManager();

    virtual ~ServerManager();

    void launchCaptivePortal();
    void setupSoftAP();
    void tick();
    void connectToWifi();

    boolean isConnected() const {
        return _isConnected;
    }

    char *getCurrentIP() {
        return _localIp;
    }

    static ServerManager &Instance() {
        static ServerManager instance;
        return instance;
    }

    void setWifiCredentials(const char *ssid, const char *password) {
        strcpy(_ssid, ssid);
        strcpy(_password, password);
    }

private:
    AsyncWebServer *server;
    DNSServer dnsServer;

    bool _connectWifi = false;
    bool _keepTrying = false;
    char _localIp[20];
    char _ssid[MAX_SSID_LENGTH];
    char _password[MAX_PASSWORD_LENGTH];
    char _deviceName[MAX_DEVICE_NAME_LENGTH];
    bool _isConnected, _isRunningCaptive;
};


#endif // SERVER__H
