#pragma once

#include <vector>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <history.h>

#define SCREEN_VCC 33

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

class Gui {
    private:
        bool display_state = true;
        uint display_refresh = 5;
        uint next_update = 0;
        Adafruit_SSD1306 display;
    
    public:
        void begin();
        void head(std::vector<String>& texts);
        void graph(History& history);

        bool readyForUpdate() const;

        void clear();
        void refresh();
        int getRefreshRate() const;
        void setRefreshRate(int refresh);

        bool state() const;
        void setState(bool state);

        // Gui elements showing WiFi connectivity
        void setup(String& hostname, String& password);
        void info(String& ip);
};