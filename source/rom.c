#include "rom.h"
#include <stdio.h>
#include "cpu.h"
#include <string.h>

char rom_path[1024] = {0};

int rom_load(const char *filename) {
    FILE *file = fopen(filename, "rb");

    if (!file) {
        perror("failed to open rom");
        return 0;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    if (size > 0x1000000) {
        printf("rom is too large!\n");
        fclose(file);
        return 0;
    }

    // clearing old memory
    memset(memory, 0, sizeof(memory));

    size_t read = fread(memory, 1, size, file);

    fclose(file);

    printf("loaded %zu bytes\n", read);
    
    strncpy(rom_path, filename, sizeof(rom_path) - 1);
    rom_path[sizeof(rom_path) - 1] = '\0';
    
    started = 1;

    return read == (size_t)size;
}