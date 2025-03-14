#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdint.h>

#define LED_BLUE 12
#define LED_RED  13
#define LED_GREEN 11
#define BUTTON_A 5
#define BUTTON_B 6
#define BUTTON_JOYSTICK 22

extern uint32_t button_a_count;
extern uint32_t button_b_count;

void initLEDs(void);
void initButtons(void);
void read_buttons(void);
void processButtonAction(uint32_t button, uint32_t buzzer_pin, uint32_t led_pin, uint32_t *counter, const char *btnName);
void joystickButtonAction(uint32_t button, uint32_t buzzer_pin_1, uint32_t buzzer_pin_2, uint32_t led_pin, uint32_t *counter_a, uint32_t *counter_b, const char *btnName);

#endif // BUTTONS_H