#include "ServerManager.h"

ServerManager::ServerManager() {
    this->server = new AsyncWebServer(80);

    _isRunningCaptive = false;
    _ssid[0] = 0;
    _password[0] = 0;
    _deviceName[0] = 0;

    WiFi.scanNetworks();

    // Setup cors headers
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Max-Age", "600");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");
}

ServerManager::~ServerManager() {
    server->end();
    delete server;
}

void ServerManager::connectToWifi() {
    _isConnected = false;
    _keepTrying = true;
    _connectWifi = false;

    char currentText[] = {"Connecting"};

    if (_isRunningCaptive) {
        _isRunningCaptive = false;
    }
    Serial.println("connecting wifi");
    Display::Instance().printScene(currentText, 10);

    delay(1000);

    WiFi.mode(WIFI_MODE_STA);
    WiFi.disconnect();
    WiFi.begin(_ssid, _password);

    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info) {
        _keepTrying = false;
    }, SYSTEM_EVENT_STA_DISCONNECTED);

    uint8_t i = 0;
    Serial.println("TRYING wifi");
    while (WiFi.status() != WL_CONNECTED && _keepTrying) {
        currentText[i++] = '.';
        currentText[i] = '\0';

        if (i + 1 == 11) i = 0;

        Display::Instance().printScene(currentText, 10);
        delay(1000);
    }

    if (WiFi.status() != WL_CONNECTED || !_keepTrying) {
        launchCaptivePortal(); // Launch again captive if esp cannot connect
        return;
    }

    Storage::Instance().setNewCredentials(_ssid, _password);
    Storage::Instance().saveWifiCredentials();

    strncpy(currentText, "Connected", 10);
    Display::Instance().printScene(currentText, 10);

    _isConnected = true;
    strncpy(_localIp, _ssid, 30);

    Serial.println(_localIp);
}

void ServerManager::setupSoftAP() {
    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAP(AP_SSID);

    IPAddress IP = WiFi.softAPIP();
    strncpy(_localIp, IP.toString().c_str(), 20);
    Serial.println(F(_localIp));

    _isRunningCaptive = true;
}

void ServerManager::launchCaptivePortal() {
    char scenesBuffer[] = "Setting up";
    Display::Instance().printScene(scenesBuffer, 10);

    setupSoftAP();

    // Setup routes
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(SD, "/public/index.html", "text/html");
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    }).setFilter(ON_AP_FILTER);

    server->onNotFound([](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(SD, "/public/index.html", "text/html");
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server->on("/api/networks", HTTP_GET, [](AsyncWebServerRequest *request) {
        String json = "[";
        int8_t n = WiFi.scanComplete();
        if (n == -2) {
            WiFi.scanNetworks(true);
        } else if (n) {
            for (int8_t i = 0; i < n; ++i) {
                if (i) json += ",";
                json += "{";
                json += "\"ssid\":\"" + WiFi.SSID(i) + "\"";
                json += "}";
            }
            WiFi.scanDelete();
            if (WiFi.scanComplete() == -2) {
                WiFi.scanNetworks(true);
            }
        }
        json += "]";
        request->send(200, "application/json", json);
    });

    server->on("/api/connect", HTTP_POST, [this](AsyncWebServerRequest *request) {
        const char *ssid = request->getParam("ssid")->value().c_str();
        const char *password = request->getParam("password")->value().c_str();
        const char *name = request->getParam("device_name")->value().c_str();

        char json[] = "{\"message\": \"trying connection\"}";

        AsyncWebServerResponse *response = request->beginResponse(200, "application/json", json);
        response->addHeader("Connection", "close");
        request->send(response);

        server->end();
        server->reset();

        _connectWifi = true;

        strncpy(_ssid, ssid, MAX_SSID_LENGTH);
        strncpy(_password, password, MAX_PASSWORD_LENGTH);
        strncpy(_deviceName, name, MAX_DEVICE_NAME_LENGTH);
    });

    server->serveStatic("/public", SD, "/public");
    server->serveStatic("/", SD, "/public");

    // Setup dns
    dnsServer.start(53, "*", WiFi.softAPIP());
    server->addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);

    server->begin();
    Serial.println("server started");
}

void ServerManager::tick() {
    if (_isRunningCaptive) {
        dnsServer.processNextRequest();
    }

    if (_connectWifi) {
        connectToWifi();
    }
}
