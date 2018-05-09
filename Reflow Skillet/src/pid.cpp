#include "parameters.h"

void update_PID_and_set_output(void) {
  g_PID_input = g_fake_temp;
  g_PID_setpoint = g_tset;

  myPID.Compute();
  Serial.print(F("g_PID_input: "));
  Serial.print(g_PID_input);
  Serial.print(F(",\t"));
  Serial.print(F("g_PID_output: "));
  Serial.print(g_PID_output);
  Serial.print(F(",\t"));
  Serial.print(F("g_PID_setpoint: "));
  Serial.print(g_PID_setpoint);

  if (millis() - g_windowStartTime >
      WINDOWSIZE) { // shift window forward by WINDOWSIZE if we have moved
                    // beyond WINDOWSIZE ms from g_windowStartTime
    g_windowStartTime += WINDOWSIZE;
  }
  if (g_PID_output < millis() - g_windowStartTime) {
    g_heating = 1;
    Serial.println(F("\tHEATING"));
  } else {
    g_heating = 0;
    Serial.println();
  }
  if (g_heating) {
    // turn on output pin (9) here
  }
  return;
};
