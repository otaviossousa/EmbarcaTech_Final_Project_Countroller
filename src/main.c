#include "init.h"
#include "joystick.h"
#include "buttons.h"
#include "buzzer.h"
#include "neopixel.h"
#include "display.h"

int main() {
    init_system();

    while (true) {
        read_joystick();
        read_buttons();
    }

    return 0;
}