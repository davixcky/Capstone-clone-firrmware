#include "ServerManager.h"

class CaptiveRequestHandler : public AsyncWebHandler {
public:
    CaptiveRequestHandler() {}

    virtual ~CaptiveRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request) {
        return true;
    }

    void handleRequest(AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", configuration_page_html);
    }
};

ServerManager::ServerManager() {
//    this->server = new AsyncWebServer(80);
}

ServerManager::~ServerManager() {
    server->end();
    delete server;
}

void ServerManager::begin() {
    setupControllers();

    // Setup cors headers
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Max-Age", "600");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");

    server->serveStatic("/public", SD, "/public");
    server->serveStatic("/", SD, "/public");

    // Pre scan to avoid 0 results in first api call
    WiFi.scanNetworks();

    server->begin();
    Serial.println("ServerManager started");
}

void ServerManager::connectToWifi(const char *ssid, const char *password) {
    _isConnected = false;
    _keepTrying = true;

    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin(ssid, password);

    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info) {
        _keepTrying = false;
    }, SYSTEM_EVENT_STA_DISCONNECTED);

    while (WiFi.status() != WL_CONNECTED && _keepTrying) {
        delay(1000);
    }

    if (WiFi.status() != WL_CONNECTED || !_keepTrying) {
        return;
    }

    _isConnected = true;
    strncpy(_localIp, WiFi.localIP().toString().c_str(), 30);

    Serial.println(_localIp);
}

void onSaveConfiguration(AsyncWebServerRequest *request) {
    AsyncWebParameter *ssid = request->getParam("ssid");
    AsyncWebParameter *password = request->getParam("password");
    AsyncWebParameter *device_name = request->getParam("device_name");

    if (!ssid || !password || !device_name) {
        request->send(400, "application/json", "{\"error\":\"some of the fields are empty\"}");
        return;
    }

    Serial.print("Connecting ...");
    WiFi.begin(ssid->value().c_str(), password->value().c_str());
    uint32_t t1 = millis();
    bool wifi_timeout = false;
    while (WiFi.status() != WL_CONNECTED) {
        delay(2000);
        Serial.print(".");
        if (millis() - t1 > 50000) //50 seconds elapsed connecting to WiFi
        {
            Serial.println();
            Serial.println("Timeout connecting to WiFi. The SSID and Password seem incorrect.");
            wifi_timeout = true;
            break;
        }
    }

    Serial.println(WiFi.status() != WL_CONNECT_FAILED);

    if (!wifi_timeout) {
        Serial.println("connected");
        Serial.println(WiFi.localIP());
        request->send(200, "application/json", "{\"status\":\"Credentials were uploaded successfully\"}");
        WiFi.softAPdisconnect(true);
        return;
    }

    request->send(400, "application/json", "{\"error\":\"credentials were incorrect\"}");
}


void ServerManager::launchCaptivePortal() {
    if (server) {
        delete server;
    }

    server = new AsyncWebServer(80);

    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    Serial.println(WiFi.softAPIP());

    // Setup routes
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
                request->send_P(200, "text/html", configuration_page_html);
                /*
                 * Use it if you want to send a gzip page instead of a PROGMEM html
                 * AsyncWebServerResponse *response = request->beginResponse(SD, "/public/index.html", "text/html")
                 * response->addHeader("Content-Encoding", "gzip");
                 * request->send(response);
                 */
            })
            .setFilter(ON_AP_FILTER);

    server->on("/bye", HTTP_GET, [this](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(200, "Turning off", "text/plain");
        request->send(response);

        this->server->end();
        WiFi.softAPdisconnect(true);
    });

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

    server->serveStatic("/public", SD, "/public");
    server->serveStatic("/", SD, "/public");

    server->on("/api/configuration", HTTP_POST, onSaveConfiguration);

    // Setup dns
    dnsServer.start(53, "*", WiFi.softAPIP());
    server->addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);

    // Setup cors headers
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Max-Age", "600");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");

    server->begin();
    Serial.println("server started");
}

void ServerManager::setupControllers() {
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
}


