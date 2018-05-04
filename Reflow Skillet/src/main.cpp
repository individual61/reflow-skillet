#include <Adafruit_GFX.h>
#include <Adafruit_MAX31856.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

///////////////////
// Buttons

#define STARTSTOP_BTN 4
#define

///////////////////
// Pro Trinket SPI Connections (common to both devices)

// MOSI/SDI 11
// MISO/SDO 12
// SCK 13

/////////////////// thermocouple (CS = 10)
// Use software SPI: CS, DI, DO, CLK
#define MAX31856_CS 10
#define MAX31856_MOSI 11
#define MAX31856_MISO 12
#define MAX31856_CLK 13

Adafruit_MAX31856 max =
    Adafruit_MAX31856(MAX31856_CS, MAX31856_MOSI, MAX31856_MISO, MAX31856_CLK);
// use hardware SPI, just pass in the CS pin
// Adafruit_MAX31856 max = Adafruit_MAX31856(10);

/////////////////// OLED
// If using software SPI (the default case):
#define OLED_MOSI 11
#define OLED_CLK 13
#define OLED_DC 8
#define OLED_CS 6
#define OLED_RESET 5
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

// Start OLED preamble
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH 16

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
// end OLED preamble

// OLED function definitions

float g_coldtemp = 0.0;
float g_thtemp = 0.0;
float g_tset = 0.0;

uint8_t g_fault = 0;

enum State_enum { running, idle, fault };
State_enum theState = idle;

void update_temps() {
  Serial.println("In update temps");
  g_fault = max.readFault();
  if (!g_fault) {
    g_coldtemp = max.readCJTemperature();
    g_thtemp = max.readThermocoupleTemperature();
    /*    Serial.print("Cold: ");
        Serial.print(g_coldtemp);
        Serial.print("\tTC: ");
        Serial.println(g_thtemp);
        */
  } else {
    theState = fault;
    Serial.println("In update temps, setting fault");
  }
}

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

// end OLED function definitions

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
  }

  delay(10);
}
