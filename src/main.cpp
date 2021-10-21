#include <Arduino.h>
#include <ServerManager/ServerManager.h>

#include "globals.h"
#include "Storage/Storage.h"
#include "Display/Display.h"

char scenesBuffer[30] = "Setting up";
uint8_t lengthBuffer = strlen(scenesBuffer);
void setup() {
    Serial.begin(115200);

    Display::Instance().begin();
    Display::Instance().setNumberScenes(1);

    Storage::init();

    ServerManager::Instance().launchCaptivePortal();

    delay(3000);
    Display::Instance().clearPixels(0, SCREEN_HEIGHT);
}

void loop() {
    // Load storage
    // If password set -> launch st mode -> if fails -> launch ap mode
    // If no password set -> launch ap mode -> receives -> try connect ->
    //                                                                      password works -> saved
    //                                                                      password doesn't works -> try again

    if (!ServerManager::Instance().isConnected()) { // Only execute tick when is not connected
        ServerManager::Instance().tick();
    }

    Display::Instance().printBasicInfo(ServerManager::Instance().getCurrentIP(), 80);
    Display::Instance().printScene(scenesBuffer, lengthBuffer);

    Display::Instance().applyChanges();
}