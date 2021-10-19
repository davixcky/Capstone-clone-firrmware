#ifndef SERVER__H
#define SERVER__H

#include "globals.h"

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SD.h>
#include <WiFi.h>
#include <DNSServer.h>

class ServerManager {
public:
    ServerManager();

    void begin();

    boolean isConnected() const {
        return _isConnected;
    }

    static ServerManager &Instance() {
        static ServerManager instance;
        return instance;
    }

private:
    AsyncWebServer *server;

    bool _keepTrying = false;
    char _localIp[20];
    bool _isConnected;

    void setupControllers();

public:
    virtual ~ServerManager();

    void launchCaptivePortal();
    void connectToWifi(const char *ssid, const char *password);


    DNSServer dnsServer;

    static void onStaDisconnected(system_event_id_t event, system_event_info_t info);
};


#endif // SERVER__H
