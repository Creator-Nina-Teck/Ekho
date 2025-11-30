#include <Arduino.h>
#include "buttons.h"
#include "config.h"

unsigned long pressStart = 0;
bool lastState = HIGH;

void initButtons() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

ButtonState readButtons() {
  bool current = digitalRead(BUTTON_PIN);

  if (current == LOW && lastState == HIGH) {
    pressStart = millis();
    lastState = LOW;
    return BUTTON_PRESSED;
  }

  if (current == HIGH && lastState == LOW) {
    lastState = HIGH;
    return BUTTON_RELEASED;
  }

  if (current == LOW && (millis() - pressStart) > 1000) {
    return BUTTON_LONG_PRESS;
  }

  return BUTTON_IDLE;
}
