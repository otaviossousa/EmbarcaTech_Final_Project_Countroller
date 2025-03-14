#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2818b.pio.h"
#include "neopixel.h"

// Array to store the state of each LED in the NeoPixel matrix
static pixel_t leds[LED_COUNT];

// PIO instance and state machine for NeoPixel control
static PIO np_pio;
static uint sm;

// Brightness level for the NeoPixel matrix
static uint8_t brightnessLevel = MAX_BRIGHTNESS;

// Initializes the NeoPixel matrix on the specified pin
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

// Sets the brightness level for the NeoPixel matrix
void setBrightness(uint8_t brightness) {
    if (brightness > MAX_BRIGHTNESS) {
        brightness = MAX_BRIGHTNESS;
    }
    brightnessLevel = brightness;
    printf("Brightness set to %d\n", brightnessLevel);
}

// Sets the color of a specific LED in the NeoPixel matrix
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
    leds[index].R = (r * brightnessLevel) / MAX_BRIGHTNESS;
    leds[index].G = (g * brightnessLevel) / MAX_BRIGHTNESS;
    leds[index].B = (b * brightnessLevel) / MAX_BRIGHTNESS;
}

// Clears all LEDs in the NeoPixel matrix
void npClear() {
    for (uint i = 0; i < LED_COUNT; ++i)
        npSetLED(i, 0, 0, 0);
}

// Writes the current LED states to the NeoPixel matrix
void npWrite() {
    for (uint i = 0; i < LED_COUNT; ++i) {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
    sleep_us(100);
}

// Gets the index of an LED based on its x and y coordinates
int getIndex(int x, int y) {
    if (y % 2 == 0) {
        return 24 - (y * 5 + x);
    } else {
        return 24 - (y * 5 + (4 - x));
    }
}

// Draws a pattern on the NeoPixel matrix
void drawPattern(int pattern[5][5][3]) {
    for (int line = 0; line < 5; line++) {
        for (int col = 0; col < 5; col++) {
            int pos = getIndex(line, col);
            npSetLED(pos, pattern[col][line][0], pattern[col][line][1], pattern[col][line][2]);
        }
    }
    npWrite();
    sleep_ms(2000);
    npClear();
}

// Displays predefined patterns on the NeoPixel matrix
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

// Turns off all LEDs in the NeoPixel matrix
void npTurnOffAll() {
    printf("Turning off all LEDs\n");
    for (uint i = 0; i < LED_COUNT; ++i) {
        npSetLED(i, 0, 0, 0); 
    }
    npWrite();
}