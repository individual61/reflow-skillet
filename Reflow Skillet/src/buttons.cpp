#include "parameters.h"

uint8_t button1state;
uint8_t button2state;
uint8_t button1prevstate;
uint8_t button2prevstate;

unsigned long timeNowButton1 = 0;
unsigned long timeNowButton2 = 0;
unsigned long lastTimePressedButton1 = 0;
unsigned long lastTimePressedButton2 = 0;

void startStopBtnPressAction(void) {
  Serial.println(F("Start/Stop button pressed."));
  if (theState == idle) {
    Serial.println(F("Switching from idle to running."));
    g_timeStepStart = millis();

    g_currentStep = 0;
    g_tset = (float)(profile[2 * g_currentStep + 1]);
    g_PID_setpoint = g_tset;

    theState = running;
    return;
  }
  if (theState == running) {
    Serial.println(F("Switching from running to idle."));
    // do transition to idle here ()
    theState = idle;
    return;
  }
}

void pauseBtnPressAction(void) {
  Serial.println(F("Pause button pressed."));
  if (theState == pause) {
    Serial.println(F("Switching from paused to running."));
    theState = running; // remember to reset integrator timer
  } else if (theState == running) {
    Serial.println(F("Switching from running to paused."));
    theState = pause; // store variables i.e. do not reset them
  }
}

void checkStartStopButton(void) {
  bool buttonState = digitalRead(STARTSTOP_BTN);
  if (buttonState == 0) // pressed
  {
    timeNowButton1 = millis();
    if (timeNowButton1 - lastTimePressedButton1 > DEBOUNCEDELAY) {
      startStopBtnPressAction();
      lastTimePressedButton1 = timeNowButton1;
    }
  }
}

void checkPauseButton(void) {
  bool buttonState = digitalRead(PAUSE_BTN);
  if (buttonState == 0) // pressed
  {
    timeNowButton2 = millis();
    if (timeNowButton2 - lastTimePressedButton2 > DEBOUNCEDELAY) {
      pauseBtnPressAction();
      lastTimePressedButton2 = timeNowButton2;
    }
  }
}
