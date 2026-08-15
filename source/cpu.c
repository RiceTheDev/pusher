#include "cpu.h"

uint8_t memory[0x1000000];
SDL_Color s_fb[256 * 256];
uint32_t pc;
int running = 1;
int paused = 0;
int started = 0;

void cpu_write(uint8_t data, uint32_t loc)
{
    if (loc < 0x1000000)
        memory[loc] = data;
}

uint8_t cpu_read(uint32_t loc) {
    return loc < 0x1000000 ? memory[loc] : 0x00;
}

void cpu_step(void) {
    uint32_t AAA = (cpu_read(pc + 0) << 16) | (cpu_read(pc + 1) << 8) | cpu_read(pc + 2);
    uint32_t BBB = (cpu_read(pc + 3) << 16) | (cpu_read(pc + 4) << 8) | cpu_read(pc + 5);
    uint32_t CCC = (cpu_read(pc + 6) << 16) | (cpu_read(pc + 7) << 8) | cpu_read(pc + 8);

    cpu_write(cpu_read(AAA), BBB);

    pc = CCC;
}