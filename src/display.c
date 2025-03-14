#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "display.h"

// Buffer for OLED display
uint8_t buf[SSD1306_BUF_LEN];

// Render area for OLED display
struct render_area frame_area = {
    .start_col = 0,
    .end_col = SSD1306_WIDTH - 1,
    .start_page = 0,
    .end_page = SSD1306_NUM_PAGES - 1
};

// Initializes the OLED display via I2C
void initDisplay() {
    printf("Initializing OLED display via I2C\n");
    i2c_init(i2c1, SSD1306_I2C_CLK * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
    SSD1306_init();
}

// Displays a message on the OLED display
void displayMessage(const char *msg) {
    calc_render_area_buflen(&frame_area);
    memset(buf, 0, SSD1306_BUF_LEN);
    WriteString(buf, 5, 0, (char *)msg);
    render(buf, &frame_area);
}

// Displays joystick information on the OLED display
void displayJoystickInfo(int pos) {
    char msg[32];
    snprintf(msg, sizeof(msg),"led position %d",pos);
    displayMessage(msg);
}