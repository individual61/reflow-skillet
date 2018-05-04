#include "parameters.h"

///////////////////////////// GLOBALS

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

// Button globalS
//
State_enum theState = idle;

///////////////////////////// END GLOBALS

void setup() {
  theState = idle;
  g_fault = 0;
  Serial.begin(115200);
  Serial.println("MAX31856 thermocouple test");

  max.begin();

  max.setThermocoupleType(MAX31856_TCTYPE_K);
  update_temps();

  // OLED stuff

  // by default, we'll generate the high voltage from the 3.3v line internally!
  // (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // init done

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.clearDisplay();
  display.display();
}

void loop() {

  switch (theState) {

  case idle:
    Serial.println("In Idle");
    update_temps();
    update_display();
    break;
  case running:
    Serial.println("in running");
    break;
  case fault:
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Fault: ");

    if (g_fault & MAX31856_FAULT_CJRANGE) {
      Serial.println("Cold Junction Range Fault");
      display.println("Cold Junction Range Fault");
    }
    if (g_fault & MAX31856_FAULT_TCRANGE) {
      Serial.println("Thermocouple Range Fault");
      display.println("Thermocouple Range Fault");
    }
    if (g_fault & MAX31856_FAULT_CJHIGH) {
      Serial.println("Cold Junction High Fault");
      display.println("Cold Junction High Fault");
    }
    if (g_fault & MAX31856_FAULT_CJLOW) {
      Serial.println("Cold Junction Low Fault");
      display.println("Cold Junction Low Fault");
    }
    if (g_fault & MAX31856_FAULT_TCHIGH) {
      Serial.println("Thermocouple High Fault");
      display.println("Thermocouple High Fault");
    }
    if (g_fault & MAX31856_FAULT_TCLOW) {
      Serial.println("Thermocouple Low Fault");
      display.println("Thermocouple Low Fault");
    }
    if (g_fault & MAX31856_FAULT_OVUV) {
      Serial.println("Over/Under Voltage Fault");
      display.println("Over/Under Voltage Fault");
    }
    if (g_fault & MAX31856_FAULT_OPEN) {
      Serial.println("Thermocouple Open Fault");
      display.println("Thermocouple Open Fault");
    }
    display.display();
    g_fault = max.readFault();
    Serial.println("In Fault state. Fault is reported as: ");
    Serial.println(g_fault);
    if (!g_fault) {
      theState = idle;
    }
    break;
  case pause:
    break;
  }

  delay(10);
}
