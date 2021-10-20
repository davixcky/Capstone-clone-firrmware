#include "Display.h"

Display::Display() {
    previousScene[0] = 0; // Initialize the array to null char

    this->display = new Adafruit_SSD1306(
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            &Wire,
            OLED_RESET
    );
}

void Display::begin() {
    if (!this->display) {
        Serial.println("Display has not been created");
        return;
    }

    if (!this->display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;) Serial.println("Try reset the board");
    }

    this->display->cp437(true);         // Use full 256 char 'Code Page 437' font
    this->printImage(MAIN_LOGO, MAIN_LOGO_WIDTH, MAIN_LOGO_HEIGHT);
}

void Display::printImage(const uint8_t *bitmap, const short width, const short height) {
    if (!this->display) {
        Serial.println("Display has not been created");
        return;
    }

    this->display->clearDisplay();
    this->display->drawBitmap(0, 0, bitmap, width, height, WHITE);
    this->display->display();
}

void Display::setNumberScenes(int totalScenes) {
    this->maxScenes = totalScenes;
    this->currentIndexScene = 0;
}

void Display::printScene(char *sceneValue, uint8_t length) {
    if (!this->display) {
        Serial.println("Display has not been created");
        return;
    }

    if (strncmp(previousScene, sceneValue, 30) == 0) {
        return;
    }

    strncpy(previousScene, sceneValue, 30);

    this->clearPixels(12);
    this->display->setCursor(8, 15);
    this->display->setTextColor(SSD1306_WHITE);
    this->display->setTextSize(2);

    uint8_t i = 0;
    uint8_t stop = length > 10 ? 7 : 10;
    for (; i < stop && sceneValue[i] != '\0'; i++) {
        this->display->write(sceneValue[i]);

    }

    if (i == stop) {
        this->display->println(F("..."));
    }
}

void Display::printCurrentScene(String *scenesValues, bool showTitle = true) {
    if (!this->display) {
        Serial.println("Display has not been created");
        return;
    }

    this->clearPixels(12);
    this->display->setTextColor(SSD1306_WHITE);
    this->display->setTextSize(1);
    this->display->setCursor(4, 5);
    if (this->titlesScenes && showTitle) {
        this->display->println(this->titlesScenes[this->currentIndexScene]);
    }

    this->display->setCursor(8, 15);
    this->display->setTextColor(SSD1306_WHITE);
    this->display->setTextSize(2);
    this->display->print(scenesValues[this->currentIndexScene]);

    this->display->setTextSize(1);
    this->display->print(" " + suffixesScenes[this->currentIndexScene]);

    if (++this->currentIndexScene >= this->maxScenes) {
        this->currentIndexScene = 0;
    }
}

void Display::setScenesTitles(String *titles) {
    this->titlesScenes = titles;
    Serial.println(titles[0]);
}

void Display::setScenesSuffix(String *suffixes) {
    this->suffixesScenes = suffixes;
}

void Display::printBasicInfo(const char *text, short batteryLevelPercent) {
    int16_t x, y;
    uint16_t w, h;

    this->clearPixels(0, 9);
    this->display->setTextColor(SSD1306_WHITE, SSD1306_BLACK);

    this->display->setTextSize(1);
    this->display->getTextBounds(text, 0, 0, &x, &y, &w, &h);

    this->display->setCursor(SCREEN_WIDTH - w - 1, 0);
    this->display->println(text);

    this->drawBatteryIndicator(batteryLevelPercent);

    this->display->drawFastHLine(0, h + 2, SCREEN_WIDTH, SSD1306_WHITE);
}

void Display::applyChanges() {
    this->display->display();
}

void Display::clearPixels(uint16_t startY, uint16_t stopY) {
    for (; startY < stopY; startY++) {
        for (uint16_t x = 0; x < SCREEN_WIDTH; x++) {
            this->display->drawPixel(x, startY, SSD1306_BLACK);
        }
    }
}

void Display::drawBatteryIndicator(short percent) {
    if (percent >= 100) percent = 100;
    if (percent < 0) percent = 0;

    short numColumns = percent * 4 / 75;
    short boxX = 2, boxY = 0, boxWidth = 23, boxHeight = 8;

    display->drawRect(boxX, boxY, boxWidth, boxHeight, SSD1306_WHITE);
    display->fillRect(25, 2, 3, 4, SSD1306_WHITE);

    for (short i = 0, startX = 4; i < numColumns && i <= 4; i++, startX += 5)
        display->fillRect(startX, 2, 4, 4, SSD1306_WHITE);

    display->setCursor(32, 0);

    if (percent < 100)
        display->println(String(percent) + "%");
    else
        display->println("OK");
}

