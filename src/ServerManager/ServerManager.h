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

class ServerManager {
public:
    ServerManager();

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

private:
    AsyncWebServer *server;

    bool _connectWifi = false;
    bool _keepTrying = false;
    char _localIp[20];
    char _ssid[MAX_SSID_LENGTH];
    char _password[MAX_PASSWORD_LENGTH];
    char _deviceName[MAX_DEVICE_NAME_LENGTH];
    bool _isConnected, _isRunningCaptive;
public:
    virtual ~ServerManager();

    void launchCaptivePortal();

    DNSServer dnsServer;

    void setupSoftAP();
    void tick();

    void connectToWifi();
};


#endif // SERVER__H
