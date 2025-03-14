#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>

#define BUZZER_PIN_1 21
#define BUZZER_PIN_2 10
#define BUZZER_FREQUENCY 10000

void pwm_init_buzzer(uint32_t pin);
void buzz(uint32_t pin, uint32_t duration_ms);
void initBuzzers(void);

#endif // BUZZER_H