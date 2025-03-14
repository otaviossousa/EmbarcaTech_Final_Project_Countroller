#ifndef NEOPIXEL_H
#define NEOPIXEL_H

#include <stdint.h>
#include "hardware/pio.h"

#define LED_COUNT 25
#define LED_PIN 7
#define MAX_BRIGHTNESS 255

typedef struct {
    uint8_t G, R, B;
} pixel_t;

void npInit(uint pin);
void setBrightness(uint8_t brightness);
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);
void npClear(void);
void npWrite(void);
int getIndex(int x, int y);
void drawPattern(int pattern[5][5][3]);
void npTurnOffAll(void);
void displayPatterns(void);

#endif // NEOPIXEL_H