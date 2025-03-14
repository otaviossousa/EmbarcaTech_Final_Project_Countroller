#include <stdio.h>
#include "init.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "buzzer.h"
#include "display.h"
#include "neopixel.h"
#include "buttons.h"

// Initializes the entire system, including display, LEDs, buttons, buzzers, and NeoPixel matrix
void init_system(void) {
    stdio_init_all();
    sleep_ms(5000);
    printf("Starting system...\n");

    // Initialize OLED Display
    adc_init();
    adc_gpio_init(26);
    adc_gpio_init(27);

    // Initialize NeoPixel Matrix
    npInit(LED_PIN);
    npClear();
    setBrightness(200);

    // Initialize LEDs
    initLEDs();

    // Initialize buttons
    initButtons();

    // Initialize buzzers
    initBuzzers();

    // Initialize I2C for OLED Display
    initDisplay();
    displayMessage("   Starting");
    displayPatterns();
    npTurnOffAll();
}