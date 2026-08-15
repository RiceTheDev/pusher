#ifndef CPU_H
#define CPU_H

#include <SDL2/SDL.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t memory[0x1000000];
extern SDL_Color s_fb[256 * 256];
extern uint32_t pc;

extern int running;
extern int paused;
extern int started;

void cpu_write(uint8_t data, uint32_t loc);
uint8_t cpu_read(uint32_t loc);

void cpu_step(void);

#ifdef __cplusplus
}
#endif

#endif // CPU_H