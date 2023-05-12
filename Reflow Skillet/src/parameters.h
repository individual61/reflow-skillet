#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "Adafruit_MAX31856_modified.h"
#include "buttons.h"
#include "display.h"
#include "pid.h"
#include "profile.h"
#include "temp.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include "PID_V1_modified.h"
#include <avr/pgmspace.h>

///////////////////
// SSR/LED output
#define OUTPUT_PIN 9
#define LED_PIN 0
///////////////////
// Buttons

#define PAUSE_BTN 4
#define STARTSTOP_BTN 3
#define DEBOUNCEDELAY 2000

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

#define TEMP_POLL_INTERVAL_MS 150

/////

/////////////////// OLED
// If using software SPI (the default case):
#define OLED_MOSI 11
#define OLED_CLK 13
#define OLED_DC 8
#define OLED_CS 6
#define OLED_RESET 5

// Start OLED preamble
//#define LOGO16_GLCD_HEIGHT 16
//#define LOGO16_GLCD_WIDTH 16

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//#if (SSD1306_LCDHEIGHT != 64)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
//#endif
// end OLED preamble

/////////////////// PID
#define PID_OUTPUTLIMIT 100.0
#define KP (100.0 / 40.0)   // denominator is temp in deg C that will elicit 100% output
#define KI 0.03
#define KD 0.0
#define WINDOWSIZE 3000     // minimum SSR cycle period in ms
#define WINDOWSIZEDIV100 30 // window size divided by 100
#define PID_SAMPLE_TIME 200 // how often PID loop is updated



/////////////////// Profile
#define INITIAL_PROFILE_TEMP 15
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
extern uint32_t g_previous_temp_read_time;

// Button globalS

// State machine globalS

enum State_enum { running, idle, fault, pause };
extern State_enum theState;

extern bool g_heartbeat;

// Profile globals
extern uint8_t g_currentStep;
extern uint32_t g_timeStepStart;
extern uint32_t g_timeStepElapsed;
extern uint32_t g_step_duration;
extern double g_t_ramp_start;
extern double g_t_ramp_end;

#define NUMBER_OF_PROFILE_STEPS 7
extern double profile_times[NUMBER_OF_PROFILE_STEPS];
extern double profile_temps[NUMBER_OF_PROFILE_STEPS];


// PID globalS
extern PID myPID;
extern bool g_heating;

extern float g_fake_temp;
extern uint32_t g_windowStartTime;

extern double g_PID_setpoint; // from profile
extern double g_PID_output;   // control relay ON time
extern double g_PID_input;    // measured temp

#endif
