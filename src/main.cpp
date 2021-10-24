#include <Arduino.h>
#include <ServerManager/ServerManager.h>

#include "globals.h"
#include "Storage/Storage.h"
#include "Display/Display.h"
#include "ServerManager/DataController.h"

TaskHandle_t TaskHandle_1;

void tickDataSensors(void *_) {
    if (!ServerManager::Instance().isConnected())
        return;

    DataController::Instance().tick();
}

void setup() {
    Serial.begin(115200);

    Display::Instance().begin();

    Storage::init();

    delay(3000);

    Display::Instance().clearPixels(0, SCREEN_HEIGHT);
    Display::Instance().printBasicInfo("Connecting", 80);

    if (Storage::Instance().containsCredentials()) {
        ServerManager::Instance().setWifiCredentials(Storage::Instance().getSSID(), Storage::Instance().getPassword());
        ServerManager::Instance().connectToWifi();
    }

    if (!ServerManager::Instance().isConnected()) { // Only execute captive when is not connected
        ServerManager::Instance().launchCaptivePortal();
    }

    xTaskCreate(tickDataSensors, "TickDataSensors", 1000, NULL, 1, &TaskHandle_1);
}

void loop() {
    // Load storage
    // If password set -> launch st mode -> if fails -> launch ap mode
    // If no password set -> launch ap mode -> receives -> try connect ->
    //                                                                      password works -> saved
    //                                                                      password doesn't works -> try again

    if (!ServerManager::Instance().isConnected()) { // Only execute tick when is not connected
        ServerManager::Instance().tick();
    } else {
        DataController::Instance().tick();
    }

    Display::Instance().printBasicInfo(ServerManager::Instance().getCurrentIP(), 80);
}