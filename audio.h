#pragma once

void initAudio();
void startRecording();
void stopRecording();
void playLastClip();

extern uint8_t audioBuffer[];
extern uint32_t audioLength;
