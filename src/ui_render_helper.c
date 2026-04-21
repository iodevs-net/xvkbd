#include "ui_render_helper.h"
#include "constants.h"
#include "layout.h"
#include <stdio.h>
#include <string.h>

void ui_render_draw_keyboard(Renderer *renderer, Keyboard *keyboard, 
                            Rectangle *key_bounds, int key_count,
                            int win_width, int win_height,
                            int menu_offset, double opacity,
                            int color_scheme_index) {
    if (!renderer || !keyboard) return;

    ColorScheme scheme = color_scheme_get(color_scheme_index);
    double font_size = win_width * FONT_SIZE_RATIO + 2;

    // 1. Draw keyboard background
    Rectangle background = {0, menu_offset, win_width, win_height - menu_offset};
    Color bg_color = {scheme.background, scheme.background, scheme.background, opacity};
    renderer_draw_rectangle(renderer, background, bg_color, KEYBOARD_CORNER_RADIUS);
    
    // 2. Draw keys
    if (key_bounds && key_count > 0) {
        Layout *layout = keyboard_get_layout(keyboard);
        for (int i = 0; i < key_count && i < layout->num_keys; i++) {
            KeyDef *key = &layout->keys[i];
            
            double key_brightness = (key->flags & KEYFLAG_MODIFIER) ? 
                                   scheme.key_modifier : scheme.key_normal;
            
            Color key_color = {key_brightness, key_brightness, key_brightness, opacity};
            renderer_draw_rectangle(renderer, key_bounds[i], key_color, KEY_CORNER_RADIUS);
            
            const char *label = keyboard_get_key_label(keyboard, i);
            if (label && label[0] != '\0') {
                bool use_light = should_use_light_text(scheme.background);
                double text_bright = use_light ? scheme.text_light : scheme.text_dark;
                Color text_color = {text_bright, text_bright, text_bright, opacity};
                
                FontSpec font = {"DejaVu Sans", font_size * 1.2, true, false};
                renderer_draw_text(renderer, label, key_bounds[i], font, text_color, 
                                  ALIGN_CENTER, VALIGN_CENTER);
            }
        }
    }
}

void ui_render_draw_menu_bar(Renderer *renderer, int win_width, 
                            double opacity, int font_size) {
    if (!renderer) return;

    Rectangle menu_bar = {0, 0, win_width, MENU_BAR_HEIGHT};
    Color menu_color = {0.01, 0.01, 0.01, opacity};
    renderer_draw_rectangle(renderer, menu_bar, menu_color, 0);
    
    char menu_text[256];
    snprintf(menu_text, sizeof(menu_text), 
            "[-] OPAC: %.0f%% [+]  [COLOR]  [FONT]  [X EXIT]", opacity * 100);
    
    FontSpec font = {"DejaVu Sans", font_size * 0.7, false, false};
    Color text_color = {1, 1, 1, opacity};
    Rectangle text_bounds = {20, 0, win_width - 40, MENU_BAR_HEIGHT};
    renderer_draw_text(renderer, menu_text, text_bounds, font, text_color, 
                      ALIGN_LEFT, VALIGN_CENTER);
}
