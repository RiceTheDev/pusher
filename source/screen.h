#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *screen;
extern SDL_Texture *render_target;
extern SDL_Color palette[256];
extern SDL_Event event;

void screen_gen_palette(void);
void screen_init(void);
void screen_render(void);
void screen_quit(void);

#ifdef __cplusplus
}
#endif

#endif // SCREEN_H