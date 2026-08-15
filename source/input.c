#include "input.h"
#include "cpu.h"
#include <SDL2/SDL_keyboard.h>

void input_get(void) {
    if (!started) return;
    
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);
    uint16_t key_mask = 0;

    if (keystate[SDL_SCANCODE_1]) key_mask |= (1 << 0x1);
    if (keystate[SDL_SCANCODE_2]) key_mask |= (1 << 0x2);
    if (keystate[SDL_SCANCODE_3]) key_mask |= (1 << 0x3);
    if (keystate[SDL_SCANCODE_4]) key_mask |= (1 << 0xC);

    if (keystate[SDL_SCANCODE_Q]) key_mask |= (1 << 0x4);
    if (keystate[SDL_SCANCODE_W]) key_mask |= (1 << 0x5);
    if (keystate[SDL_SCANCODE_E]) key_mask |= (1 << 0x6);
    if (keystate[SDL_SCANCODE_R]) key_mask |= (1 << 0xD);

    if (keystate[SDL_SCANCODE_A]) key_mask |= (1 << 0x7);
    if (keystate[SDL_SCANCODE_S]) key_mask |= (1 << 0x8);
    if (keystate[SDL_SCANCODE_D]) key_mask |= (1 << 0x9);
    if (keystate[SDL_SCANCODE_F]) key_mask |= (1 << 0xE);

    if (keystate[SDL_SCANCODE_Z]) key_mask |= (1 << 0xA);
    if (keystate[SDL_SCANCODE_X]) key_mask |= (1 << 0x0);
    if (keystate[SDL_SCANCODE_C]) key_mask |= (1 << 0xB);
    if (keystate[SDL_SCANCODE_V]) key_mask |= (1 << 0xF);

    memory[0] = (key_mask >> 8) & 0xFF;
    memory[1] = key_mask & 0xFF;
}
