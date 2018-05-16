#include "parameters.h"

void advance_to_next_step(void) {
  g_currentStep++;
  if (g_currentStep == NUMBER_OF_PROFILE_STEPS) {
    g_currentStep = 0;
    theState = idle;
    Serial.println(F("Program completed."));
  } else {
    g_tset = (float)(profile[2 * g_currentStep + 1]);
    g_timeStepStart = millis();
    //    Serial.print(F("Starting step "));
    //    Serial.print(g_currentStep);
    ////    Serial.print(F(" of "));
    //    Serial.println(NUMBER_OF_PROFILE_STEPS);
  }
}
void reset_profile(void){};

// todo: make variable for start time
// call advance after that time
