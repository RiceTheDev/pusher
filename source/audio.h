#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
    
void audio_init(void);
void audio_update_frame(void);
void audio_quit(void);

extern int8_t audio_debug_buffer[256];

#ifdef __cplusplus
}
#endif

#endif // AUDIO_H