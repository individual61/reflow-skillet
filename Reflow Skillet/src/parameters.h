#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "buttons.h"
#include "display.h"
#include "temp.h"
#include <Adafruit_GFX.h>
#include <Adafruit_MAX31856.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

///////////////////
// Buttons

#define PAUSE_BTN 4
#define STARTSTOP_BTN 3
#define DEBOUNCEDELAY 300

/////////////////// SPI
// Pro Trinket SPI Connections (common to both devices)

// MOSI/SDI 11
// MISO/SDO 12
// SCK 13

/////////////////// Thermocouple
// Use software SPI: CS, DI, DO, CLK
#define MAX31856_CS 10
#define MAX31856_MOSI 11
#define MAX31856_MISO 12
#define MAX31856_CLK 13

/////

/////////////////// OLED
// If using software SPI (the default case):
#define OLED_MOSI 11
#define OLED_CLK 13
#define OLED_DC 8
#define OLED_CS 6
#define OLED_RESET 5

// Start OLED preamble
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH 16

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
// end OLED preamble

//////////////////////// GLOBALS
// Display globals
extern Adafruit_SSD1306 display;

// Thermocouple globals
extern Adafruit_MAX31856 max;
// use hardware SPI, just pass in the CS pin
// Adafruit_MAX31856 max = Adafruit_MAX31856(10);
extern float g_coldtemp;
extern float g_thtemp;
extern float g_tset;
extern uint8_t g_fault;

// Button globalS
enum State_enum { running, idle, fault, pause };
extern State_enum theState;
#endif
