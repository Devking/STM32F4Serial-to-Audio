// code provided from A. Finkelmeyer
// from http://www.mind-dump.net/configuring-the-stm32f4-discovery-for-audio

#ifndef SPEAKER_H
#define SPEAKER_H

#include <stdint.h>


void init_speaker(void); // initialize the speaker (audio output)
void send_to_speaker(int16_t audio_sample); // send one audio sample to the speaker

#endif
