#pragma once
#include <stdint.h>

#define HIST_SIZE 50
const int TEMPERATURE = 0;
const int HUMIDITY = 1;

class History {
    private:
        float temp_array[HIST_SIZE];
        float humi_array[HIST_SIZE];

        int pointer = 0;

    public:
        History();
        void add(float temperature, float humidity);
        float get_latest(int type) const;
        float get(int index, int type) const;
        int get_index() const;

        bool operator<(int amount);
        bool operator>(int amount);
};