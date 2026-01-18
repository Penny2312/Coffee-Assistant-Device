#include "Buttons.h"
#include <Arduino.h>

#define BUTTON_NEXT 19
#define BUTTON_SELECT 4

void Buttons::init() {
  pinMode(BUTTON_NEXT, INPUT_PULLUP);
  pinMode(BUTTON_SELECT, INPUT_PULLUP);
}

bool Buttons::nextPressed() {
  return digitalRead(BUTTON_NEXT) == LOW;
}

bool Buttons::selectPressed() {
  return digitalRead(BUTTON_SELECT) == LOW;
}
