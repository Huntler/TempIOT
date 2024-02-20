#pragma once

#include <sstream>
#include <WString.h>
#include <SPIFFS.h>
#include <history.h>
#include <gui.h>
#include <WiFi.h>

namespace web {
    void localTime(String& time) {
        // get the current time from a NTP server
        struct tm timeinfo;
        if(!getLocalTime(&timeinfo)){
            Serial.println("Failed to obtain time");
            return;
        }

        time = String(timeinfo.tm_hour) + ":" + String(timeinfo.tm_min);
        if (timeinfo.tm_min < 10) {
            time = String(timeinfo.tm_hour) + ":0" + String(timeinfo.tm_min);
        }
    }

    void read(const String& filename, String& content) {
        // Read the content of a file from the SPIFFS filesystem
        File file = SPIFFS.open(filename);
        if(!file){
            Serial.println("Failed to open file for reading");
            return;
        }

        while(file.available()){
            content = content + file.readString();
        }
        
        file.close();
    }

    void parseDashboard(const History& history, const Gui& gui, String& strHtml) {
        read("/dashboard.html", strHtml);

        // Current temperature in card header
        float temp = history.get_latest(TEMPERATURE);
        float humi = history.get_latest(HUMIDITY);

        strHtml.replace("{{ temp }}", String(temp));
        strHtml.replace("{{ humi }}", String(humi));

        // History temperature in graph
        std::stringstream tempValues;
        tempValues << "[";
        for (int i = 0; i < history.get_index() - 1; i++) {
            tempValues << history.get(i, TEMPERATURE) << ",";
        }
        tempValues << history.get(history.get_index() - 1, TEMPERATURE) << "];";

        std::stringstream humiValues;
        std::stringstream xValues;
        humiValues << "[";
        xValues << "[";
        for (int i = 0; i < history.get_index() - 1; i++) {
            humiValues << history.get(i, HUMIDITY) << ",";
            xValues << i << ",";
        }
        humiValues << history.get(history.get_index() - 1, HUMIDITY) << "];";
        xValues << history.get_index() - 1 << "];";

        strHtml.replace("{{ x_values }}", xValues.str().c_str());
        strHtml.replace("{{ temp_values }}", tempValues.str().c_str());
        strHtml.replace("{{ humi_values }}", humiValues.str().c_str());

        // GUI update frequency
        // TODO
        strHtml.replace("{{ web-frequency }}", "1");
        strHtml.replace("{{ frequency }}", String(gui.getRefreshRate()));

        // Webaddress
        strHtml.replace("{{ url }}", WiFi.localIP().toString());

        if (gui.state()) {
            strHtml.replace("{{ display_state }}", "checked");
        } else {
            strHtml.replace("{{ display_state }}", "");
        }
    }
}
