#ifndef THEME_H
#define THEME_H

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned char SometypeMono_ttf[];
extern unsigned int SometypeMono_len;

void setup_imgui_dark_theme(void);
void setup_imgui_light_theme(void);

#ifdef __cplusplus
}
#endif

#endif // THEME_H
