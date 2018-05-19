#include "parameters.h"
#ifndef PROFILE_H
#define PROFILE_H

void advance_to_next_step(void);
void init_profile(void);
uint32_t calculate_total_time_remaining(void);
void calculate_tset_from_ramp(double previous_t, double next_t, double this_step_duration_seconds, double elapsed_time_seconds);

#endif
