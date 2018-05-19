#include "parameters.h"

void update_temps() {
        //  Serial.println("In update temps");
        uint32_t time_now = millis();
        if (time_now - g_previous_temp_read_time > TEMP_POLL_INTERVAL_MS)
        { // Only poll temp if > 90 ms interval
                g_previous_temp_read_time = time_now;
/////
                g_fault = max.readFault();
                if (!g_fault) {

                        g_thtemp = max.readThermocoupleTemperature();
                        g_coldtemp = max.readCJTemperature();
                        g_fake_temp = analogRead(0);
                        g_fake_temp = map(g_fake_temp, 0, 1023, 95, 5);

                } else {
                        theState = fault;
                        Serial.println(F("In update temps, setting fault"));
                }
        }
}
