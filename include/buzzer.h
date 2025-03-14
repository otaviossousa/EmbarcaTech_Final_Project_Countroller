#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>

// Pin definitions for buzzers
#define BUZZER_PIN_1 21
#define BUZZER_PIN_2 10
#define BUZZER_FREQUENCY 10000

// Initializes the PWM for the buzzer on the specified pin
void pwm_init_buzzer(uint32_t pin);

// Generates a beep sound on the specified pin for the given duration in milliseconds
void buzz(uint32_t pin, uint32_t duration_ms);

// Initializes both buzzers
void initBuzzers(void);

#endif // BUZZER_H