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
  if (theState == idle) {
    // do transition to running here ()
    theState = running;
  }
  if (theState == running) {
    // do transition to idle here ()
    theState = idle;
  }
}

void pauseBtnPressAction(void) {
  if (theState == pause) {
    theState = running; // remember to reset integrator timer
  } else if (theState == running) {
    theState = pause;
  }
}

void checkStartStopButton(void) {
  bool buttonState = digitalRead(STARTSTOP_BTN);
  if (buttonState == 0) // pressed
  {
    timeNowButton1 = millis();
    if (timeNowButton1 - lastTimePressedButton1 > DEBOUNCEDELAY) {
      button1PressAction();
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
      button2PressAction();
      lastTimePressedButton2 = timeNowButton2;
    }
  }
}
