#include <Arduino.h>

#include "globals.h"
#include "Storage/Storage.h"
#include "ServerManager/ServerManager.h"

void setup() {
    Serial.begin(115200);

    Storage::init();
    // ServerManager::Instance().launchCaptivePortal();
}

void loop() {
    // Load storage
    // If password set -> launch st mode -> if fails -> launch ap mode
    // If no password set -> launch ap mode -> receives -> try connect ->
    //                                                                      password works -> saved
    //                                                                      password doesn't works -> try again


}