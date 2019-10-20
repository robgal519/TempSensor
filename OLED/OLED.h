#ifndef OLED_H
#define OLED_H

#define OLED_COLUMNS 128
#define OLED_PAGES 8
#define OLED_PAGE_SIZE 8

#include <stdint.h>
#include "Framebuffer.h"

typedef struct OLED OLED;

OLED *oled_create(void);

void oled_destroy(OLED *oled);

void oled_display_on(OLED *oled);

void oled_display_off(OLED *oled);

Framebuffer oled_access_framebuffer(OLED *oled);

void oled_lock_display(OLED *oled);

void oled_unlock_display(OLED *oled);

void oled_send_framebuffer(OLED *oled);

void oled_clear_framebuffer(OLED *oled);

#endif
