#ifndef DRAWER_H
#define DRAWER_H

#include "Drawable.h"
#include "../OLED/Framebuffer.h"

#define DRAWER_PAGE_SIZE 8

void drawer_draw_bitmap(Framebuffer framebuffer, Drawable drawable);

#endif
