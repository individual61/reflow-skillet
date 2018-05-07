#include "parameters.h"

// Modified Adafruit thermocouple library per
// https://forums.adafruit.com/viewtopic.php?f=19&t=108896
//////////////////////// GLOBALS

// Display globals
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// Thermocouple globals
Adafruit_MAX31856 max =
    Adafruit_MAX31856(MAX31856_CS, MAX31856_MOSI, MAX31856_MISO, MAX31856_CLK);
// use hardware SPI, just pass in the CS pin
// Adafruit_MAX31856 max = Adafruit_MAX31856(10);
float g_coldtemp = 0.0;
float g_thtemp = 0.0;
float g_tset = 0.0;

uint8_t g_fault = 0;

uint16_t g_previous_temp_read_time = 0;

// Button globalS
//

// State Machine globals
State_enum theState = idle;

///////////////////////////// END GLOBALS

void setup() {
  Serial.begin(115200);

  theState = idle;
  g_fault = 0;

  // Thermocouple stuff
  max.begin();
  max.setThermocoupleType(MAX31856_TCTYPE_K);
  max.config();

  update_temps();

  // OLED stuff
  display.begin(SSD1306_SWITCHCAPVCC);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.clearDisplay();
  display.display();
}

uint16_t time_now = 0;
void loop() {

  switch (theState) {

  case idle:
    Serial.println("In Idle");
    checkPauseButton();
    checkStartStopButton();

    time_now = millis();
    if (time_now - g_previous_temp_read_time >
        TEMP_POLL_INTERVAL_MS) { // Only poll temp if > 90 ms interval
      g_previous_temp_read_time = time_now;

      time_now = millis();
      update_temps();
      Serial.print(F("update_temps() time: "));
      Serial.println((uint16_t)(millis() - time_now));
    }

    time_now = millis();
    Serial.print(F("update_display() time: "));
    Serial.println((uint16_t)(millis() - time_now));
    update_display();
    break;
  case running:
    Serial.println("in running");
    checkPauseButton();
    checkStartStopButton();
    break;
  case fault:
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(F("Fault: "));

    if (g_fault & MAX31856_FAULT_CJRANGE) {
      Serial.println(F("Cold Junction Range Fault"));
      display.println(F("Cold Junction Range Fault"));
    }
    if (g_fault & MAX31856_FAULT_TCRANGE) {
      Serial.println(F("Thermocouple Range Fault"));
      display.println(F("Thermocouple Range Fault"));
    }
    if (g_fault & MAX31856_FAULT_CJHIGH) {
      Serial.println(F("Cold Junction High Fault"));
      display.println(F("Cold Junction High Fault"));
    }
    if (g_fault & MAX31856_FAULT_CJLOW) {
      Serial.println(F("Cold Junction Low Fault"));
      display.println(F("Cold Junction Low Fault"));
    }
    if (g_fault & MAX31856_FAULT_TCHIGH) {
      Serial.println(F("Thermocouple High Fault"));
      display.println(F("Thermocouple High Fault"));
    }
    if (g_fault & MAX31856_FAULT_TCLOW) {
      Serial.println(F("Thermocouple Low Fault"));
      display.println(F("Thermocouple Low Fault"));
    }
    if (g_fault & MAX31856_FAULT_OVUV) {
      Serial.println(F("Over/Under Voltage Fault"));
      display.println(F("Over/Under Voltage Fault"));
    }
    if (g_fault & MAX31856_FAULT_OPEN) {
      Serial.println(F("Thermocouple Open Fault"));
      display.println(F("Thermocouple Open Fault"));
    }
    display.display();
    g_fault = max.readFault();
    Serial.println(F("In Fault state. Fault is reported as: "));
    Serial.println(g_fault);
    if (!g_fault) {
      theState = idle;
    }
    break;
  case pause:
    checkPauseButton();
    checkStartStopButton();
    update_display();

    break;
  }

  delay(10);
}
