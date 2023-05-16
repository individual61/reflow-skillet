#include "parameters.h"

//#define SSD1306_128_64 1
//#if (SSD1306_LCDHEIGHT != 64)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
//#endif

#define TEST 0

void update_display() {
  display.clearDisplay();


  // Tact display
  display.setCursor(0, 0);
  display.print(F("Tact: "));
  display.println(g_thtemp, 1);
#if TEST == 0
  // Tset display
  display.print(F("Tset: "));
  if ((theState == running) || (theState == pause)) {
    display.println(g_tset, 1);
  } else if (theState == idle) {
    display.println(F("--"));
  }

  // Step #
  display.println();
  display.print(F("Step: "));
  if ((theState == running) || (theState == pause)) {
    display.print(g_currentStep+1);
    display.print(F(" of "));
    display.println(NUMBER_OF_PROFILE_STEPS);
  } else if (theState == idle) {
    display.println(F("--"));
  }


  display.print(F("Time remaining: "));
  if ((theState == running) || (theState == pause)) {
  display.print( calculate_total_time_remaining(), 1);
  display.println(F(" s"));
  }
  else if (theState == idle) {
    display.println(F("--"));
  }

  // fake temp
//  display.println(g_fake_temp);
//  display.println(g_PID_output);

  // Output
  display.setCursor(0, 56); // 57 is lowest
  display.print(F("Out: "));
  display.print(g_PID_output, 1);
  display.print(F("\%"));

  // COld junction temp
  //display.setCursor(0, 57);
  //display.print(F("Tcj:  "));
  //display.print(g_coldtemp, 1);

  // State display
  display.setCursor(80, 0);
  if (theState == idle)
    display.print(F("Idle"));
  else if (theState == running)
    display.print(F("Running"));
  else if (theState == fault)
    display.print(F("Fault"));
  else if (theState == pause)
    display.print(F("Paused"));

  // Heating display
  if (g_heating) { // remove !
    display.fillRoundRect(72, 53, 50, 11, 4, WHITE);
    display.setCursor(77, 55);
    display.setTextColor(BLACK);
    display.print(F("HEATING"));
    display.setTextColor(WHITE);
  }
  // Heartbeat display
  if (g_heartbeat) {
    display.drawRect(126, 62, 2, 2, WHITE);
  }
#endif
  display.display();
}

void display_fault(void)
{
if (g_fault & MAX31856_FAULT_CJRANGE) {
  Serial.println(F("# Cold Junction Range Fault"));
  display.println(F("Cold Jct Rng Fault"));
}
if (g_fault & MAX31856_FAULT_TCRANGE) {
  Serial.println(F("# Thermocouple Range Fault"));
  display.println(F("Thc Rng Fault"));
}
if (g_fault & MAX31856_FAULT_CJHIGH) {
  Serial.println(F("# Cold Junction High Fault"));
  display.println(F("Cold Jct High Fault"));
}
if (g_fault & MAX31856_FAULT_CJLOW) {
  Serial.println(F("# Cold Junction Low Fault"));
  display.println(F("Cold Jct Low Fault"));
}
if (g_fault & MAX31856_FAULT_TCHIGH) {
  Serial.println(F("# Thermocouple High Fault"));
  display.println(F("Thc High Fault"));
}
if (g_fault & MAX31856_FAULT_TCLOW) {
  Serial.println(F("# Thermocouple Low Fault"));
  display.println(F("Thc Low Fault"));
}
if (g_fault & MAX31856_FAULT_OVUV) {
  Serial.println(F("# Over/Under Voltage Fault"));
  display.println(F("Ovr/Und V Fault"));
}
if (g_fault & MAX31856_FAULT_OPEN) {
  Serial.println(F("# Thermocouple Open Fault"));
  display.println(F("Thc Open Fault"));
}
display.display();
}
