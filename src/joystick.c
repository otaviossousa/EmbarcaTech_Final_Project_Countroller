#include "joystick.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "neopixel.h"
#include "display.h"

void read_joystick(void) {
    adc_select_input(0);
    uint adc_y_raw = adc_read();
    adc_select_input(1);
    uint adc_x_raw = adc_read();
    const uint adc_max = (1 << 12) - 1;
    uint col = adc_x_raw * 5 / (adc_max + 1);
    uint row = 4 - (adc_y_raw * 5 / (adc_max + 1));
    npClear();
    int pos = getIndex(col, row);
    displayJoystickInfo(pos);
    npSetLED(pos, 255, 0, 0);
    npWrite();
}