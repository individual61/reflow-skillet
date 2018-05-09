#include "parameters.h"

// Modified Adafruit thermocouple library per
// https://forums.adafruit.com/viewtopic.php?f=19&t=108896

// If the screen looks stretched vertically, and skips lines, then the Adafruit
// library header file must be edited as parameters
// https://learn.adafruit.com/monochrome-oled-breakouts/arduino-library-and-examples

//////////////////////// GLOBALS

// Display globals
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// Thermocouple globals
Adafruit_MAX31856 max =
    Adafruit_MAX31856(MAX31856_CS, MAX31856_MOSI, MAX31856_MISO, MAX31856_CLK);
float g_coldtemp = 0.0;
float g_thtemp = 0.0;
float g_tset = 0.0;
uint8_t g_fault = 0;
uint16_t g_previous_temp_read_time = 0;

// Button globalS
//

// Profile globals
uint8_t g_currentStep = 0;
uint16_t g_timeStepStart = 0;
uint16_t g_timeStepElapsed = 0;

uint16_t profile[] = {
    30, 50, // 0
    30, 60, // 1
    30, 70, // 2
    30, 20, // 3
    30, 0   // 4
};

// State Machine globals
State_enum theState = idle;
bool g_heartbeat = 0;

// PID globalS
bool g_heating = 0;

float g_fake_temp = 25.0;

uint16_t g_windowStartTime = 0;

double g_PID_setpoint = 0; // from profile
double g_PID_output = 0;   // control relay ON time
double g_PID_input = 0;    // measured temp

PID myPID(&g_PID_input, &g_PID_output, &g_PID_setpoint, KP, KI, KD, DIRECT);

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

  // Profile stuff
  g_currentStep = 0;

  // OLED stuff
  display.begin(SSD1306_SWITCHCAPVCC);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.clearDisplay();
  display.display();

  // PID stuff
  g_heating = 0;
  g_windowStartTime = millis();
  myPID.SetOutputLimits(0, WINDOWSIZE);
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(PID_SAMPLE_TIME);
}

uint16_t time_now = 0;
void loop() {
  g_heartbeat = !g_heartbeat;
  switch (theState) {

  case idle: ///////////////////////////// IDLE
    checkPauseButton();
    checkStartStopButton();

    time_now = millis();
    if (time_now - g_previous_temp_read_time >
        TEMP_POLL_INTERVAL_MS) { // Only poll temp if > 90 ms interval
      g_previous_temp_read_time = time_now;
      update_temps();
    }

    update_display();
    break;

  case running: ///////////////////////////// RUNNING
    checkPauseButton();
    checkStartStopButton();

    time_now = millis();
    if (time_now - g_previous_temp_read_time >
        TEMP_POLL_INTERVAL_MS) { // Only poll temp if > 90 ms interval
      g_previous_temp_read_time = time_now;
      update_temps();
    }

    update_PID_and_set_output();
    update_display();

    g_timeStepElapsed = millis() - g_timeStepStart;
    if (g_timeStepElapsed > (1000 * profile[2 * g_currentStep])) {
      advance_to_next_step();
    }

    break;

  case fault: ///////////////////////////// FAULT

    // SET OUTPUT PIN LOW HERE
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
  case pause: ///////////////////////////// PAUSED
    checkPauseButton();
    checkStartStopButton();

    time_now = millis();
    if (time_now - g_previous_temp_read_time >
        TEMP_POLL_INTERVAL_MS) { // Only poll temp if > 90 ms interval
      g_previous_temp_read_time = time_now;
      update_temps();
    }
    update_PID_and_set_output();
    update_display();

    break;
  }

  delay(10);
}
