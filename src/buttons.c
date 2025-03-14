#include <stdio.h>
#include "pico/stdlib.h"
#include "buttons.h"
#include "buzzer.h"
#include "display.h"

uint32_t button_a_count = 0;
uint32_t button_b_count = 0;

void initLEDs(void) {
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_put(LED_BLUE, false);
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_put(LED_RED, false);
    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_put(LED_GREEN, false);
}

void initButtons(void) {
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
    gpio_init(BUTTON_JOYSTICK);
    gpio_set_dir(BUTTON_JOYSTICK, GPIO_IN);
    gpio_pull_up(BUTTON_JOYSTICK);
}

void processButtonAction(uint32_t button, uint32_t buzzer_pin, uint32_t led_pin, uint32_t *counter, const char *btnName) {
    (*counter)++;
    printf("Button %s pressed %d times\n", btnName, *counter);
    gpio_put(led_pin, true);
    char msg[32];
    snprintf(msg, sizeof(msg), "   Button %s: %u", btnName, *counter);
    displayMessage(msg);
    buzz(buzzer_pin, 1000);
    gpio_put(led_pin, false);
}

void joystickButtonAction(uint32_t button, uint32_t buzzer_pin_1, uint32_t buzzer_pin_2, uint32_t led_pin, uint32_t *counter_a, uint32_t *counter_b, const char *btnName) {
    printf("Joystick pressed. Count A = %u, B = %u\n", *counter_a, *counter_b);
    gpio_put(led_pin, true);
    char msg[32];
    if (btnName[0] != '\0') {
        snprintf(msg, sizeof(msg), "%s: A %u, B %u", btnName, *counter_a, *counter_b);
    } else {
        snprintf(msg, sizeof(msg), "   A: %u, B: %u", *counter_a, *counter_b);
    }
    displayMessage(msg);
    buzz(buzzer_pin_1, 1000);
    buzz(buzzer_pin_2, 1000);
    gpio_put(led_pin, false);
}

void read_buttons(void) {
    static bool prev_button_a = false;
    static bool prev_button_b = false;
    static bool prev_button_joystick = false;

    bool current_button_a = !gpio_get(BUTTON_A);
    bool current_button_b = !gpio_get(BUTTON_B);

    if (current_button_a && !prev_button_a) {
        processButtonAction(BUTTON_A, BUZZER_PIN_1, LED_BLUE, &button_a_count, "A");
        sleep_ms(1000);
    }
    if (current_button_b && !prev_button_b) {
        processButtonAction(BUTTON_B, BUZZER_PIN_2, LED_RED, &button_b_count, "B");
        sleep_ms(1000);
    }

    bool current_button_joystick = !gpio_get(BUTTON_JOYSTICK);
    if (current_button_joystick && !prev_button_joystick) {
        joystickButtonAction(BUTTON_JOYSTICK, BUZZER_PIN_1, BUZZER_PIN_2, LED_GREEN, &button_a_count, &button_b_count, "");
        sleep_ms(1000);
    }

    prev_button_a = current_button_a;
    prev_button_b = current_button_b;
    prev_button_joystick = current_button_joystick;
}