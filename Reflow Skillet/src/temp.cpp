#include "parameters.h"

void update_temps() {
  //  Serial.println("In update temps");
  g_fault = max.readFault();
  if (!g_fault) {

    g_thtemp = max.readThermocoupleTemperature();
    g_coldtemp = max.readCJTemperature();
    g_fake_temp = analogRead(0);
    /*    Serial.print("Cold: ");
        Serial.print(g_coldtemp);
        Serial.print("\tTC: ");
        Serial.println(g_thtemp);
        */

  } else {
    theState = fault;
    Serial.println(F("In update temps, setting fault"));
  }
}
