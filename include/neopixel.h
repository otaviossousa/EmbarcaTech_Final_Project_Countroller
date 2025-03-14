#ifndef NEOPIXEL_H
#define NEOPIXEL_H

#include <stdint.h>
#include "hardware/pio.h"

// Definitions for NeoPixel matrix
#define LED_COUNT 25
#define LED_PIN 7
#define MAX_BRIGHTNESS 255

// Structure representing a pixel with RGB values
typedef struct {
    uint8_t G, R, B;
} pixel_t;

// Initializes the NeoPixel matrix on the specified pin
void npInit(uint pin);

// Sets the brightness level for the NeoPixel matrix
void setBrightness(uint8_t brightness);

// Sets the color of a specific LED in the NeoPixel matrix
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);

// Clears all LEDs in the NeoPixel matrix
void npClear(void);

// Writes the current LED states to the NeoPixel matrix
void npWrite(void);

// Gets the index of an LED based on its x and y coordinates
int getIndex(int x, int y);

// Draws a pattern on the NeoPixel matrix
void drawPattern(int pattern[5][5][3]);

// Displays predefined patterns on the NeoPixel matrix
void displayPatterns(void);

// Turns off all LEDs in the NeoPixel matrix
void npTurnOffAll(void);

#endif // NEOPIXEL_H