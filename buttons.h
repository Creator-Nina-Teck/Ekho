#pragma once

enum ButtonState {
  BUTTON_IDLE,
  BUTTON_PRESSED,
  BUTTON_RELEASED,
  BUTTON_LONG_PRESS
};

void initButtons();
ButtonState readButtons();
