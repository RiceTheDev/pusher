#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include "cpu.h"
#include "screen.h"
#include "audio.h"
#include "input.h"
#include "ui/ui.h"
#include "ui/theme.h"

int main(int argc, char *argv[]) {
    screen_init();
    audio_init();

    setup_imgui_dark_theme();

    pc = ((uint32_t)memory[2] << 16) | ((uint32_t)memory[3] << 8) | memory[4];
    printf("initial pc: %06X\n", pc);

    const double target_frame_time = 1000.0 / 60.0;
    Uint32 frame_start_ticks = SDL_GetTicks();
    while (running)
    {
        Uint32 frame_start = SDL_GetTicks();

        while (SDL_PollEvent(&event))
        {
            ui_poll_events(&event);
            
            if (event.type == SDL_QUIT)
                running = 0;

            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_ESCAPE)
                running = 0;
        }

        if (!paused && started) input_get();


        pc = ((uint32_t)memory[2] << 16) | ((uint32_t)memory[3] << 8) | memory[4];

        if (started && !paused) {
            for (int i = 0; i < 0x10000; i++){
                cpu_step();
            }
        }
        
        audio_update_frame();

        screen_render();
        Uint32 frame_elapsed = SDL_GetTicks() - frame_start_ticks;
        if (frame_elapsed < target_frame_time) {
            SDL_Delay((Uint32)(target_frame_time - frame_elapsed));
        }
    }

    ui_quit();
    screen_quit();
    audio_quit();
    return 0;
}