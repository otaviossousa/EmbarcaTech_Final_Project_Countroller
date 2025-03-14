#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

#define I2C_SDA_PIN 14
#define I2C_SCL_PIN 15

void initDisplay(void);
void displayMessage(const char *msg);
void displayJoystickInfo(int pos);
void displayPatterns(void);

#endif // DISPLAY_H