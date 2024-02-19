#include <history.h>

History::History() {

}

void History::add(float temperature, float humidity) {
    // Adds an entry (temperature, humidity) to the history and
    // increases the pointer. Old history entries might get overwritten
    if (pointer == HIST_SIZE) {        
        // Rotate values of the array to have one free spot to add in
        // the new reading.
        for (int i = 1; i < HIST_SIZE; i++) {
            temp_array[i - 1] = temp_array[i];
            humi_array[i - 1] = humi_array[i];
        }

        temp_array[HIST_SIZE - 1] = temperature;
        humi_array[HIST_SIZE - 1] = humidity;
    } else {
        temp_array[pointer] = temperature;
        humi_array[pointer] = humidity;
        pointer++;
    }
}

float History::get_latest(int type) const {
    // Returns the latest temperature and humidity reading as a 2-element
    // float array.
    if (type == 0)
        return temp_array[pointer - 1];
    if (type == 1)
        return humi_array[pointer - 1];
    return 0.0;
}

float History::get(int index, int type) const {
    if (type == 0)
        return temp_array[index];
    if (type == 1)
        return humi_array[index];
    return 0.0;
}

int History::get_index() const {
    return pointer;
}

bool History::operator<(int amount) {
    return pointer < amount;
}

bool History::operator>(int amount) {
    return pointer > amount;
}