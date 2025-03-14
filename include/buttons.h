#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdint.h>

// Pin definitions for LEDs and buttons
#define LED_BLUE 12
#define LED_RED  13
#define LED_GREEN 11
#define BUTTON_A 5
#define BUTTON_B 6
#define BUTTON_JOYSTICK 22

// Global counters for buttons
extern uint32_t button_a_count;
extern uint32_t button_b_count;

// Initializes the LEDs
void initLEDs(void);

// Initializes the buttons
void initButtons(void);

// Reads the state of the buttons and performs corresponding actions
void read_buttons(void);

// Processes the button action, increments the counter, and displays the message on the OLED display
void processButtonAction(uint32_t button, uint32_t buzzer_pin, uint32_t led_pin, uint32_t *counter, const char *btnName);

// Processes the joystick button action, increments the counters, and displays the message on the OLED display
void joystickButtonAction(uint32_t button, uint32_t buzzer_pin_1, uint32_t buzzer_pin_2, uint32_t led_pin, uint32_t *counter_a, uint32_t *counter_b, const char *btnName);

#endif // BUTTONS_H