#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2818b.pio.h"
#include "neopixel.h"

static pixel_t leds[LED_COUNT];
static PIO np_pio;
static uint sm;
static uint8_t brightnessLevel = MAX_BRIGHTNESS;

void npInit(uint pin) {
    printf("Initializing NeoPixel on PIO with pin %d\n", pin);
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;
    sm = pio_claim_unused_sm(np_pio, false);
    if (sm < 0) {
        np_pio = pio1;
        sm = pio_claim_unused_sm(np_pio, true);
    }
    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);
    for (uint i = 0; i < LED_COUNT; ++i) {
        leds[i].R = 0;
        leds[i].G = 0;
        leds[i].B = 0;
    }
}

void setBrightness(uint8_t brightness) {
    if (brightness > MAX_BRIGHTNESS) {
        brightness = MAX_BRIGHTNESS;
    }
    brightnessLevel = brightness;
    printf("Brightness set to %d\n", brightnessLevel);
}

void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
    leds[index].R = (r * brightnessLevel) / MAX_BRIGHTNESS;
    leds[index].G = (g * brightnessLevel) / MAX_BRIGHTNESS;
    leds[index].B = (b * brightnessLevel) / MAX_BRIGHTNESS;
}

void npClear() {
    for (uint i = 0; i < LED_COUNT; ++i)
        npSetLED(i, 0, 0, 0);
}

void npWrite() {
    for (uint i = 0; i < LED_COUNT; ++i) {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
    sleep_us(100);
}

int getIndex(int x, int y) {
    if (y % 2 == 0) {
        return 24 - (y * 5 + x);
    } else {
        return 24 - (y * 5 + (4 - x));
    }
}

void drawPattern(int pattern[5][5][3]) {
    for (int linha = 0; linha < 5; linha++) {
        for (int coluna = 0; coluna < 5; coluna++) {
            int posicao = getIndex(linha, coluna);
            npSetLED(posicao, pattern[coluna][linha][0], pattern[coluna][linha][1], pattern[coluna][linha][2]);
        }
    }
    npWrite();
    sleep_ms(2000);
    npClear();
}

void displayPatterns() {
    printf("Displaying patterns on LED matrix\n");
    int IF[5][5][3] = {
        {{255, 0, 0}, {0, 0, 0}, {0, 255, 0}, {0, 255, 0}, {0, 255, 0}},
        {{0, 255, 0}, {0, 0, 0}, {0, 255, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 255, 0}, {0, 0, 0}, {0, 255, 0}, {0, 255, 0}, {0, 255, 0}},
        {{0, 255, 0}, {0, 0, 0}, {0, 255, 0}, {0, 0, 0}, {0, 0, 0}},
        {{0, 255, 0}, {0, 0, 0}, {0, 255, 0}, {0, 0, 0}, {0, 0, 0}}
    };

    int PI[5][5][3] = {
        {{0, 255, 0}, {0, 255, 0}, {0, 255, 0}, {0, 0, 0}, {255, 0, 0}},
        {{0, 255, 0}, {0, 0, 0}, {0, 255, 0}, {0, 0, 0}, {0, 255, 0}},
        {{0, 255, 0}, {0, 255, 0}, {0, 255, 0}, {0, 0, 0}, {0, 255, 0}},
        {{0, 255, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 255, 0}},
        {{0, 255, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 255, 0}}
    };

    int Heart[5][5][3] = {
        {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
        {{255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}},
        {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}},
        {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
        {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}}
    };

    drawPattern(IF);
    drawPattern(PI);
    drawPattern(Heart);
}

void npTurnOffAll() {
    printf("Turning off all LEDs\n");
    for (uint i = 0; i < LED_COUNT; ++i) {
        npSetLED(i, 0, 0, 0); 
    }
    npWrite();
}