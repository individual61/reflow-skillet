#include "parameters.h"

void update_display() {
  Serial.println("In update display");
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Tact: ");
  display.println(g_thtemp);
  display.print("Tset: ");
  if (g_tset == 0.0) {
    display.println("--");
  } else {
    display.println(g_tset);
  }
  display.setCursor(0, 55);
  display.print("Tcj:  ");
  display.print(g_coldtemp);
  display.setCursor(80, 0);
  if (theState == idle)
    display.print("Idle");
  else if (theState == running)
    display.print("Running");
  else if (theState == fault)
    display.print("Fault");

  display.display();
}
