#pragma once
#include <Arduino.h>

void initStorage();
void saveRecording();
void playStoredRecording();
uint32_t getStoredLength();
void clearStorage();

// used by audio.cpp
extern uint8_t audioBuffer[];
extern uint32_t audioLength;
