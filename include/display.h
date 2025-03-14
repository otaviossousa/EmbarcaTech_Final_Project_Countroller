#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

// I2C pin definitions for the OLED display
#define I2C_SDA_PIN 14
#define I2C_SCL_PIN 15

// Initializes the OLED display via I2C
void initDisplay(void);

// Displays a message on the OLED display
void displayMessage(const char *msg);

// Displays joystick information on the OLED display
void displayJoystickInfo(int pos);

// Displays predefined patterns on the OLED display
void displayPatterns(void);

#endif // DISPLAY_H