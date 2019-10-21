#ifndef DRAWER_H
#define DRAWER_H

#include "Drawable.h"
#include "TextDrawable.h"
#include "../OLED/Framebuffer.h"

#define DRAWER_PAGE_SIZE 8

void drawer_draw_bitmap(Framebuffer framebuffer, Drawable drawable);

void drawer_draw_text(Framebuffer framebuffer, TextDrawable text_drawable);

#endif
