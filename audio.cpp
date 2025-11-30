#include <Arduino.h>
#include "audio.h"
#include "config.h"

uint8_t audioBuffer[16000]; // Enough for ~1 sec, expandable
uint32_t audioLength = 0;
bool isRecording = false;

void initAudio() {
  // Placeholder, depends on your I2S mic
}

void startRecording() {
  isRecording = true;
  audioLength = 0;
}

void stopRecording() {
  isRecording = false;
}

void playLastClip() {
  for (uint32_t i = 0; i < audioLength; i++) {
    analogWrite(SPK_PIN, audioBuffer[i]);
    delayMicroseconds(62); // simple fake playback timing
  }
}
