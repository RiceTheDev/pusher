#ifndef ROM_H
#define ROM_H

#ifdef __cplusplus
extern "C" {
#endif

extern char rom_path[1024];

int rom_load(const char *filename);

#ifdef __cplusplus
}
#endif

#endif // ROM_H