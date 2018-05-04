#include "parameters.h"

uint8_t button1state;
uint8_t button2state;
uint8_t button1prevstate;
uint8_t button2prevstate;

unsigned long timeNowButton1 = 0;
unsigned long timeNowButton2 = 0;
unsigned long lastTimePressedButton1 = 0;
unsigned long lastTimePressedButton2 = 0;

void startStopBtnPressAction(void) {}

void pauseBtnPressAction(void) { theState = idle; }

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
