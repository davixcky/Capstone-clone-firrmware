#include <Arduino.h>
#include <ServerManager/ServerManager.h>

#include "globals.h"
#include "Storage/Storage.h"
#include "Display/Display.h"

Display display;
char scenesBuffer[30] = "Setting up";
uint8_t lengthBuffer = strlen(scenesBuffer);
void setup() {
    Serial.begin(115200);

    display.begin();
    display.setNumberScenes(1);

    Storage::init();

    ServerManager::Instance().launchCaptivePortal();

    delay(3000);
    display.clearPixels(0, SCREEN_HEIGHT);
}

void loop() {
    // Load storage
    // If password set -> launch st mode -> if fails -> launch ap mode
    // If no password set -> launch ap mode -> receives -> try connect ->
    //                                                                      password works -> saved
    //                                                                      password doesn't works -> try again

    ServerManager::Instance().tick();

    if (ServerManager::Instance().isRunningCaptive()) {
        display.printBasicInfo(ServerManager::Instance().getCurrentIP(), 80);
    }

    display.printScene(scenesBuffer, lengthBuffer);

    display.applyChanges();
    delay(300);
}