# EmbarcaTech Final Project Countroller

## Project Context
This project represents the final assessment of the EmbarcaTech program's Unit 7. The program covered fundamental concepts of Embedded Systems, C programming for embedded systems, microcontroller architecture and programming, and Internet of Things (IoT). This interactive system demonstrates the practical application of these concepts in a functional embedded system.

The project showcases:
- Embedded Systems fundamentals through hardware integration
- C programming implementation
- Microcontroller programming using Raspberry Pi Pico W
- Interactive IoT-ready system architecture

## Overview
This project implements an interactive system using a Raspberry Pi Pico microcontroller, featuring LED matrix control, joystick input, button interactions, OLED display output, and buzzer feedback.

## Hardware Components
- Raspberry Pi Pico W
- 5x5 NeoPixel LED Matrix (25 LEDs)
- SSD1306 OLED Display (128x64)
- 2 Buzzers
- 2 Push Buttons (A and B)
- Analog Joystick with button
- 3 Status LEDs (Red, Green, Blue)

## Pin Configuration

| Components       | Raspberry Pi Pico w Pins  |
|------------------|---------------------------|
| LED NeoPixel     | GPIO 7                    |
| Display OLED SDA | GPIO 14                   |
| Display OLED SCL | GPIO 15                   |
| Joystick (Eixo X)| GPIO 26 (ADC0)            |
| Joystick (Eixo Y)| GPIO 27 (ADC1)            |
| Button A         | GPIO 5                    |
| Button B         | GPIO 6                    |
| Joystick Button  | GPIO 22                   |
| Buzzer 1         | GPIO 21                   |
| Buzzer 2         | GPIO 10                   |
| LED Blue         | GPIO 12                   |
| LED Red          | GPIO 13                   |
| LED Green        | GPIO 11                   |

## Features
1. Interactive LED Matrix Control
   - Joystick-controlled LED position
   - Pattern display (IF, PI, Heart)

2. Button System
   - Counter tracking for buttons A and B
   - LED and Buzzers feedback for button presses

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
- Sound: `pwm_init_buzzer()`, `buzz()`

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

## Acknowledgments and References

This project was influenced by several example projects from the [BitDogLab-C](https://github.com/BitDogLab/BitDogLab-C), specifically:

- display_oled
- Joystick_led
- neopixel_pio
- button-buzzer
- button_led_rgb

Special thanks to Professor Jivago for his educational content and tutorials available on his YouTube channel: [Jivago](https://www.youtube.com/@profjivago9719). His videos were instrumental in understanding the concepts and implementation of various components in this project.

I would like to express my gratitude to all professors involved in the EmbarcaTech program who dedicated their time to guide us through synchronous classes, providing valuable insights and clarifications that were essential for my learning journey.

The final implementation combines and builds upon these reference projects to create an integrated interactive system.

## Project Structure Decision
The structure of this project was decided based on best practices for modular code and project organization in C. The following resources were instrumental in guiding the organization of this project:

1. [Modular Code and How to Structure an Embedded C Project](https://www.microforum.cc/blogs/entry/46-modular-code-and-how-to-structure-an-embedded-c-project/)
   - Emphasizes the importance of separating different functionalities into distinct modules to improve code readability and maintainability.

2. [C Project Structure by Jack Wetherell](https://github.com/JackWetherell/c-project-structure)
   - Provides a clear example of how to organize a C project with separate directories for headers, source files, and libraries.

3. [How to Structure C Projects: My Experience & Best Practices by Luca Vallini](https://www.lucavall.in/blog/how-to-structure-c-projects-my-experience-best-practices)
   - Shares practical advice on organizing C projects based on real-world experience, highlighting the benefits of a well-structured project.

By following these guidelines, the project is organized in a way that makes it easy to navigate, understand, and maintain. Each module is encapsulated in its own directory, with clear separation between headers, source files, and external libraries.

## Folder Structure
The project is organized into several folders to separate different types of files and functionalities:

- `include`: Header files for the project
- `lib`: Libraries used in the project
- `pio`: Files related to the Raspberry Pi Pico's Programmable Input/Output (PIO)
- `src`: Contains the main source files for the project
- `tools`: Includes files for generating symbols and patterns

## Code Structure
The project contains special files in the `tools` folder for symbol and pattern generation:

- `drawls.c`: File generated using https://www.piskelapp.com/ containing the definitions of symbols (IF, PI, Heart) displayed on the LED matrix
- `piskel_convert.c`: Responsible for converting and processing patterns into the appropriate LED matrix format

These files are fundamental for displaying the animations and symbols shown during system initialization.

The main source files are located in the `src` folder:

- `main.c`: Main file containing the main loop and system initialization
- `init.c`: Initializes the system components
- `buttons.c`: Handles button inputs and actions
- `buzzer.c`: Controls the buzzers
- `display.c`: Manages the OLED display
- `joystick.c`: Reads the joystick input
- `neopixel.c`: Controls the NeoPixel LED matrix

## Building Project
After cloning the repository, you will need to create a `build` directory to compile the project. The following commands can be useful to set up and build the project:

```bash
cd EmbarcaTech_Final_Project_Countroller
mkdir build
cd build
cmake -G "Ninja" ..
cmake --build .
```

## Demo Video

Watch a 30-second demonstration of the system's key features in action.

[Demo Video](https://youtu.be/H-Bg4PmH9-0?si=BmzUuu4fzx-mDRKq)

