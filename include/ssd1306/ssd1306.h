#include "ssd1306_i2c.h"
extern void calc_render_area_buflen(struct render_area *area);
extern void SSD1306_send_cmd(uint8_t cmd);
extern void SSD1306_send_cmd_list(uint8_t *buf, int num);
extern void SSD1306_send_buf(uint8_t buf[], int buflen);
extern void SSD1306_init();
extern void SSD1306_scroll(bool on);
extern void render(uint8_t *buf, struct render_area *area);
extern void SetPixel(uint8_t *buf, int x, int y, bool on);
extern void DrawLine(uint8_t *buf, int x0, int y0, int x1, int y1, bool on);
extern void WriteChar(uint8_t *buf, int16_t x, int16_t y, uint8_t ch);
extern void WriteString(uint8_t *buf, int16_t x, int16_t y, char *str);