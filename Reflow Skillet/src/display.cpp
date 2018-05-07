#include "parameters.h"

void update_display() {
  //  Serial.println("In update display");
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(F("Tact: "));
  display.println(g_thtemp);
  display.print(F("Tset: "));
  if (g_tset == 0.0) {
    display.println(F("--"));
  } else {
    display.println(g_tset);
  }
  display.setCursor(0, 55);
  display.print(F("Tcj:  "));
  display.print(g_coldtemp);
  display.setCursor(80, 0);
  if (theState == idle)
    display.print(F("Idle"));
  else if (theState == running)
    display.print(F("Running"));
  else if (theState == fault)
    display.print(F("Fault"));
  else if (theState == pause)
    display.print(F("Paused"));

  if (g_heartbeat) {
    display.drawRect(126, 62, 2, 2, WHITE);
  }

  display.display();
}
