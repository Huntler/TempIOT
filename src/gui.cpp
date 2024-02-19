#include <gui.h>

void Gui::begin() {
    display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

    // Send energy to the display and wait until it is up.
    pinMode(SCREEN_VCC, OUTPUT);
    digitalWrite(SCREEN_VCC, HIGH);
    delay(1000);

    // Check if display is available
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }
}

void Gui::head(std::vector<String>& texts) {
    if (texts.size() == 0 || texts.size() > 2) {
        return;
    }
    
    int size = 3 - texts.size();
    display.setTextSize(size);
    display.setTextColor(WHITE);

    for (int i = 0; i < texts.size(); i++) {
        display.setCursor((int) (128 - texts[i].length() * size * 5) / 2 - 5, 8 * i);
        display.println(texts[i]);
    }
}

void Gui::graph(History& history) {
    if (history < 2) {
    return;
    }

    int offset = 4;
    float x_steps = 64 / HIST_SIZE;
    for (int i = 1; history > i; i++) {
        int x1 = (int) x_steps * (i - 1) + offset;
        int t1 = (int) 52 - 32 * (history.get(i - 1, TEMPERATURE) / 100);
        int h1 = (int) 52 - 32 * (history.get(i - 1, HUMIDITY) / 100);

        int x2 = (int) x_steps * i + offset;
        int t2 = (int) 52 - 32 * (history.get(i, TEMPERATURE) / 100);
        int h2 = (int) 52 - 32 * (history.get(i, HUMIDITY) / 100);

        // Serial.println("temp: " + String(temp_array[i]) + "; x: " + String(x2) + "; y: " + String(y2));
        display.drawLine(x1, t1, x2, t2, WHITE);
        display.drawLine(x1 + 64, h1, x2 + 64, h2, WHITE);
    }

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(17, 56);
    size_t width = display.println(String(history.get_latest(TEMPERATURE)) + "C");

    display.setCursor(71, 56);
    display.println(String(history.get_latest(HUMIDITY)) + "%");
}

bool Gui::state() const {
    return display_state;
}

void Gui::setState(bool state) {
    display_state = state;
}

void Gui::clear() {
    display.clearDisplay();
}

void Gui::refresh() {
    if (!display_state) {
        display.clearDisplay();
    }

    display.display();
    delay(display_refresh);
}

void Gui::setup(String& hostname, String& password) {
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.setCursor(0, 16);
    display.println("SSID:");
    display.setCursor(8, 28);
    display.println(hostname);

    display.setCursor(0, 40);
    display.println("Password:");
    display.setCursor(8, 52);
    display.println(password);    
}