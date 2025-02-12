# Project EmbarcaTech - Raspberry Pi Pico Interactive System

## Overview
This project implements an interactive system using a Raspberry Pi Pico microcontroller, featuring LED matrix control, joystick input, button interactions, OLED display output, and buzzer feedback.

## Hardware Components
- Raspberry Pi Pico
- 5x5 NeoPixel LED Matrix (25 LEDs)
- SSD1306 OLED Display
- 2 Buzzers
- 2 Push Buttons (A and B)
- Analog Joystick with button
- 3 Status LEDs (Red, Green, Blue)

## Pin Configuration
### LEDs
- Blue LED: GPIO 12
- Red LED: GPIO 13
- Green LED: GPIO 11
- NeoPixel Data: GPIO 7

### Buttons
- Button A: GPIO 5
- Button B: GPIO 6
- Joystick Button: GPIO 22

### Buzzers
- Buzzer 1: GPIO 21
- Buzzer 2: GPIO 10

### Display
- I2C SDA: GPIO 14
- I2C SCL: GPIO 15

### Joystick
- X-axis: ADC1 (GPIO 27)
- Y-axis: ADC0 (GPIO 26)

## Features
1. Interactive LED Matrix Control
   - Joystick-controlled LED position
   - Pattern display (IF, PI, Heart symbols)
   - Adjustable brightness

2. Button System
   - Counter tracking for buttons A and B
   - Debounce protection
   - LED feedback for button presses

3. Display Output
   - OLED status display
   - Button press counter display
   - Joystick position feedback

4. Sound Feedback
   - Dual buzzer system
   - PWM-controlled sound generation

## Functions
- LED Matrix Control: `npInit()`, `npSetLED()`, `drawPattern()`
- Display: `initDisplay()`, `displayMessage()`
- Button Handling: `processButtonAction()`, `joystickButtonAction()`
- Sound: `pwm_init_buzzer()`, `beep()`

## Setup and Usage
1. Initialize system components
2. Display startup patterns
3. Enter main loop:
   - Read joystick position
   - Update LED matrix
   - Process button inputs
   - Update display
   - Provide audio feedback

## Dependencies
- Raspberry Pi Pico SDK
- SSD1306 OLED Library
- WS2812B (NeoPixel) PIO Implementation
