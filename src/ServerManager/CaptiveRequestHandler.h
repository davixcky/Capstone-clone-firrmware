#ifndef CAPTIVEREQUESTHANDLER__H
#define CAPTIVEREQUESTHANDLER__H

#include "globals.h"

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SD.h>
#include <WiFi.h>
#include <DNSServer.h>

class CaptiveRequestHandler : public AsyncWebHandler {
public:
    CaptiveRequestHandler() {}

    virtual ~CaptiveRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request) {
        return true;
    }

    void handleRequest(AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(SD, "/public/index.html", "text/html");
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    }
};

#endif //CAPTIVEREQUESTHANDLER__H
