#ifndef UI_H
#define UI_H
#include <SDL2/SDL.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

extern bool show_mem_editor;
extern bool show_debug_info;
extern bool show_audio_debug;
extern bool show_emu_controls;
extern bool show_about_dialog;

extern bool dark_theme;

void ui_init(void);

void ui_poll_events(SDL_Event *event);

void ui_begin_frame(void);
void ui_create_main_menu_bar(void);
void ui_create_emu_window(void);
void ui_create_emu_memview(void);
void ui_create_emu_controls(void);
void ui_create_emu_audio_debug(void);
void ui_create_emu_debug_info(void);

void ui_render(void);

void ui_quit(void);

#ifdef __cplusplus
}
#endif

#endif // UI_H