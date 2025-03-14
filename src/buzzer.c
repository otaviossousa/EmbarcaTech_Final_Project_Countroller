#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "buzzer.h"

void initBuzzers(void) {
    gpio_init(BUZZER_PIN_1);
    gpio_set_dir(BUZZER_PIN_1, GPIO_OUT);
    gpio_init(BUZZER_PIN_2);
    gpio_set_dir(BUZZER_PIN_2, GPIO_OUT);
    pwm_init_buzzer(BUZZER_PIN_1);
    pwm_init_buzzer(BUZZER_PIN_2);
}

void pwm_init_buzzer(uint32_t pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (BUZZER_FREQUENCY * 4096));
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, 0);
}

void buzz(uint32_t pin, uint32_t duration_ms) {
    uint slice_num = pwm_gpio_to_slice_num(pin);
    pwm_set_gpio_level(pin, 2048);
    sleep_ms(duration_ms);
    pwm_set_gpio_level(pin, 0);
    sleep_ms(100);
}