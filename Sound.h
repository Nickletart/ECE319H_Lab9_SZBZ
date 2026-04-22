#ifndef SOUND_H
#define SOUND_H
#include <stdint.h>

void Sound_Init(void);
void Sound_Start(uint32_t period);
void Play_Audio(int trackid);

#endif
