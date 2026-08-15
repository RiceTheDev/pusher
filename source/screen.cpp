#include "screen.h"
#include "cpu.h"
#include "ui/imgui/backends/imgui_impl_sdl2.h"
#include "ui/ui.h"
#include <cstdio>
#include <inttypes.h>

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *render_target;
SDL_Texture *screen;
SDL_Color palette[256];
SDL_Event event;

void screen_gen_palette(void)
{
    Uint8 r, g, b;
    Uint8 i = 0;
    
    for (int i = 0; i < 256; i++) {
        if (i < 216) {
            uint8_t r = ((i / 36) % 6) * 0x33;
            uint8_t g = ((i / 6) % 6) * 0x33;
            uint8_t b = (i % 6) * 0x33;
            
            palette[i] = (SDL_Color){ r, g, b, 255 };
        } else {
            palette[i] = (SDL_Color){ 0, 0, 0, 255 };
        }
    }
}

void screen_init(void)
{
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("BytePusher", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    screen = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        256,
        256
    );

    render_target = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_TARGET,
        256,
        256
    );

    ui_init();

    screen_gen_palette();
}

void screen_render(void)
{
    uint32_t fb_addr = (uint32_t)memory[5] << 16;

    for (int i = 0; i < 256 * 256; i++) {
        uint8_t pixel = memory[fb_addr + i];
        s_fb[i] = palette[pixel];
    }

    SDL_UpdateTexture(screen, NULL, s_fb, 256 * sizeof(SDL_Color));

    SDL_SetRenderTarget(renderer, render_target);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, screen, NULL, NULL);
    SDL_SetRenderTarget(renderer, NULL);

    ui_begin_frame();

    ui_create_main_menu_bar();
    ui_create_emu_window();
    
    if (show_mem_editor) ui_create_emu_memview();
    if (show_emu_controls) ui_create_emu_controls();
    if (show_audio_debug) ui_create_emu_audio_debug();
    if (show_debug_info) ui_create_emu_debug_info();

    if (!dark_theme) SDL_SetRenderDrawColor(renderer, 232, 229, 225, 170);
    else SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);
    
    ui_render();
}

void screen_quit(void)
{
    SDL_DestroyTexture(render_target);
    SDL_DestroyTexture(screen);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}