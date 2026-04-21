#ifndef UI_RENDER_HELPER_H
#define UI_RENDER_HELPER_H

#include "renderer.h"
#include "keyboard.h"
#include "colors.h"

void ui_render_draw_keyboard(Renderer *renderer, Keyboard *keyboard, 
                            Rectangle *key_bounds, int key_count,
                            int win_width, int win_height,
                            int menu_offset, double opacity,
                            int color_scheme_index);

void ui_render_draw_menu_bar(Renderer *renderer, int win_width, 
                            double opacity, int font_size);

#endif // UI_RENDER_HELPER_H
