#ifndef SPEAKER_H
#define SPEAKER_H

#include <stdint.h>


void init_speaker(void); // initialize the speaker (audio output)
void send_to_speaker(int16_t audio_sample); // send one audio sample to the speaker

#endif
