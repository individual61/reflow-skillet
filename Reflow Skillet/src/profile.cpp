#include "parameters.h"

void init_profile(void)
{
  g_timeStepStart = millis();
  g_currentStep = 0;
  g_t_ramp_start = 15;
  g_t_ramp_end = (float)(profile[2 * g_currentStep + 1]);
  g_step_duration = profile[2 * g_currentStep];
}


void advance_to_next_step(void) {
  g_currentStep++;
  if (g_currentStep == NUMBER_OF_PROFILE_STEPS) {
    g_currentStep = 0;
    theState = idle;
    Serial.println(F("Program completed."));
  } else {
    g_t_ramp_start = g_t_ramp_end;
    g_t_ramp_end = (float)(profile[2 * g_currentStep + 1]);
    g_PID_setpoint = g_tset;
    g_timeStepStart = millis();
    //    Serial.print(F("Starting step "));
    //    Serial.print(g_currentStep);
    ////    Serial.print(F(" of "));
    //    Serial.println(NUMBER_OF_PROFILE_STEPS);
  }
}

void reset_profile(void){};

void calculate_tset_from_ramp(void)
{
  g_tset =
    g_PID_setpoint = g_tset;
};


uint32_t calculate_total_time_remaining(void)
{
  uint32_t accum = 0;
  for(int i = g_currentStep; i < NUMBER_OF_PROFILE_STEPS; i++)
    {
      accum = accum + profile[2 * i];
    }
  accum = accum - (g_timeStepElapsed / 1000.0);
  return accum;
}


// 2 * g_currentStep + 1
// 2 * (NUMBER_OF_PROFILE_STEPS - 1) + 1
// 2 * NUMBER_OF_PROFILE_STEPS - 2 + 1
// 2 * NUMBER_OF_PROFILE_STEPS - 1
