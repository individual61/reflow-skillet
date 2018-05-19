#include "parameters.h"

void init_profile(void)
{
	g_timeStepElapsed = 0;
	g_timeStepStart = millis();
	g_currentStep = 0;
	g_t_ramp_start = INITIAL_PROFILE_TEMP;
	g_t_ramp_end = profile_temps[0];
	g_step_duration = profile_times[0];
}

void advance_to_next_step(void)
{
	g_currentStep++;
	if (g_currentStep == NUMBER_OF_PROFILE_STEPS - 1)
	{
		theState = idle;
		g_currentStep = 0;
		theState = idle;
		Serial.println(F("Program completed."));
		return;
	} else {
		g_timeStepStart = millis();
		g_timeStepElapsed = 0;
		g_t_ramp_start = profile_temps[g_currentStep-1];
		g_t_ramp_end = profile_temps[g_currentStep];
		g_step_duration = profile_times[g_currentStep];


		Serial.print(F("Starting step "));
		Serial.print(g_currentStep);
		Serial.print(F(" of "));
		Serial.println(NUMBER_OF_PROFILE_STEPS);
		return;
	}
}

//	calculate_tset_from_ramp(g_t_ramp_start, g_t_ramp_end, profile_times[g_currentStep], (double)(g_timeStepElapsed/1000.0));
void calculate_tset_from_ramp(double previous_t, double next_t, double this_step_duration_seconds, double elapsed_time_seconds)
{
	g_tset = previous_t + elapsed_time_seconds*(next_t-previous_t)/this_step_duration_seconds;
	g_PID_setpoint = g_tset;
};


uint32_t calculate_total_time_remaining(void)
{
	uint32_t accum = 0;
	for(int i = g_currentStep; i < NUMBER_OF_PROFILE_STEPS; i++)
	{
		accum = accum + profile_times[i];
	}
	accum = accum - (g_timeStepElapsed / 1000.0);
	return accum;
}
