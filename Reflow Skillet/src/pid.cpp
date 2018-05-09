#include "parameters.h"

void update_PID_and_set_output(void) {
  myPID.Compute();
  if (millis() - g_windowStartTime > WINDOWSIZE) { // adjust relay ON window
    g_windowStartTime += WINDOWSIZE;
  }
  if (g_PID_output < millis() - g_windowStartTime) {
    g_heating = 1;
  }

  else {
    g_heating = 0;
  }
  return;
};
