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

    static ServerManager &Instance() {
        static ServerManager instance;
        return instance;
    }

private:
    AsyncWebServer *server;

    void setupControllers();



public:
    virtual ~ServerManager();

    void launchCaptivePortal();

    IPAddress getLocalIp();
    DNSServer dnsServer;
};


#endif // SERVER__H
