#include <Arduino.h>
#include "storage.h"

// --- Fake flash storage (for prototype) ---
static uint8_t storedData[20000];     // space for ~2 seconds audio
static uint32_t storedLength = 0;

void initStorage() {
  // If using actual flash chips later, initialize here.
}

void saveRecording() {
  if (audioLength == 0) {
    Serial.println("No audio to save.");
    return;
  }

  storedLength = min(audioLength, (uint32_t)20000);
  memcpy(storedData, audioBuffer, storedLength);

  Serial.print("Saved recording, length = ");
  Serial.println(storedLength);
}

uint32_t getStoredLength() {
  return storedLength;
}

void playStoredRecording() {
  if (storedLength == 0) {
    Serial.println("No stored audio.");
    return;
  }

  for (uint32_t i = 0; i < storedLength; i++) {
    analogWrite(SPK_PIN, storedData[i]);
    delayMicroseconds(62);
  }

  Serial.println("Playback complete.");
}

void clearStorage() {
  storedLength = 0;
}
