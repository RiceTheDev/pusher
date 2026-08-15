#include <stdint.h>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"
#include "audio.h"
#include "cpu.h"
#include <string.h>

static ma_device g_audio_device;
int8_t audio_debug_buffer[256] = {0};

static uint8_t ring[2048] = {0};
static volatile uint32_t head = 0, tail = 0;

static uint32_t ring_size(void) {
    return (head >= tail) ? (head - tail) : (sizeof(ring) - (tail - head));
}

void audio_update_frame(void) {
    while (ring_size() > 256) {
        ma_sleep(1);
    }

    uint32_t addr = ((uint32_t)memory[6] << 16) | ((uint32_t)memory[7] << 8);
    memcpy(audio_debug_buffer, &memory[addr], 256);

    for (int i = 0; i < 256; i++) {
        ring[head] = memory[addr + i] ^ 0x80;
        head = (head + 1) % sizeof(ring);
    }
}

void audio_callback(ma_device* device, void* output, const void* input, ma_uint32 frame_count) {
    uint8_t *out = (uint8_t*)output;

    for (ma_uint32 i = 0; i < frame_count; i++) {
        if (tail != head) {
            out[i] = ring[tail];
            tail = (tail + 1) % sizeof(ring);
        } else {
            out[i] = 0x80;
        }
    }
    (void)input;
}

void audio_init(void) {
    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format = ma_format_u8;
    config.playback.channels = 1;
    config.sampleRate = 15360;
    config.dataCallback = audio_callback;

    if (ma_device_init(NULL, &config, &g_audio_device) == MA_SUCCESS)
        ma_device_start(&g_audio_device);
}

void audio_quit(void) {
    ma_device_uninit(&g_audio_device);
}