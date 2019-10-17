#ifndef OLED_H
#define OLED_H

#define COLUMNS 128
#define PAGES 8

#include <stdint.h>

typedef struct OLED OLED;

OLED *oled_create(void);

void oled_destroy(OLED *oled);

void oled_display_on(OLED *oled);

void oled_display_off(OLED *oled);

uint8_t *oled_access_framebuffer(OLED *oled);

void oled_lock_display(OLED *oled);

void oled_unlock_display(OLED *oled);

void oled_send_framebuffer(OLED *oled);

void oled_clear_framebuffer(OLED *oled);

#endif
