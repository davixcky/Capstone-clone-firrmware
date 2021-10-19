#include <Arduino.h>
#include <ServerManager/ServerManager.h>
#include <Display/Display.h>

#include "globals.h"
#include "Storage/Storage.h"



void setup() {
    Serial.begin(115200);

    Storage::init();
    ServerManager::Instance().connectToWifi("WeWork", "P@ssw0rd");

    delay(3000);
}

void loop() {
    // Load storage
    // If password set -> launch st mode -> if fails -> launch ap mode
    // If no password set -> launch ap mode -> receives -> try connect ->
    //                                                                      password works -> saved
    //                                                                      password doesn't works -> try again
}