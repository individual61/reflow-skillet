#include "parameters.h"

void update_PID_and_set_output(void) {
  g_PID_input = g_fake_temp;
  g_PID_setpoint = g_tset;

  myPID.Compute();
//  Serial.print(F("g_PID_input: "));
//  Serial.print(g_PID_input);
//  Serial.print(F(",\t"));
//  Serial.print(F("g_PID_output: "));
//  Serial.print(g_PID_output);
//  Serial.print(F(",\t"));
//  Serial.print(F("g_PID_setpoint: "));
//  Serial.print(g_PID_setpoint);

uint32_t time_elapsed_in_window;
time_elapsed_in_window = (millis() - g_windowStartTime);
// millis() - g_windowStartTime is the time elapsed in this window
if (time_elapsed_in_window >
    WINDOWSIZE) { // shift window forward by WINDOWSIZE if we have moved
                  // beyond WINDOWSIZE ms from g_windowStartTime
  g_windowStartTime = g_windowStartTime + WINDOWSIZE;
}
if (g_PID_output * WINDOWSIZEDIV100 < time_elapsed_in_window) // off time
{
  g_heating = 0;
  digitalWrite(OUTPUT_PIN, LOW);
//
} else // on time
{
  g_heating = 1;
  //  Serial.println(F("\tHEATING"));
  digitalWrite(OUTPUT_PIN, HIGH);
}

return;
};
