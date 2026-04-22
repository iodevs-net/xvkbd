/*
 * 0-Board Virtual Keyboard
 * Copyright (c) 2026 Leonardo Vergara <leonardovergaramarin@gmail.com>
 * Licensed under the MIT License.
 */
#include "ui_render_helper.h"
#include "constants.h"
#include "layout.h"
#define XK_MISCELLANY
#include <X11/keysym.h>
#include <stdio.h>
#include <string.h>

// Determine if a key label is a single unicode symbol (modifier icons)


static Color color_with_opacity(Color c, double opacity) {
    return (Color){c.red, c.green, c.blue, c.alpha * opacity};
}

/**
 * ui_render_draw_keyboard: The core rendering logic.
 * 
 * STRATEGY (Dual-Pass / Zero-Resource):
 * 1. Static Pass (draw_dynamic = false):
 *    Renders keys in their normal state. This is called once per layout/layer change
 *    to populate a background cache (Pixmaps). 
 * 
 * 2. Dynamic Pass (draw_dynamic = true):
 *    Renders ONLY the keys that are currently pressed or active (Modifiers).
 *    This overlay is drawn every frame (30fps) on top of the static cache.
 * 
 * This separation allows us to use expensive BitBlt operations for the bulk
 * of the UI, while keeping high responsiveness for interactions.
 */
void ui_render_draw_keyboard(Renderer *renderer, Keyboard *keyboard,
                            Rectangle *key_bounds, KeyVisualMetadata *key_metadata,
                            int key_count, int win_width, int win_height,
                            int menu_offset, double opacity,
                            ColorScheme scheme,
                            const char *font_family,
                            bool draw_dynamic) {
    if (!renderer || !keyboard || !key_bounds || !key_metadata) return;

    Layout *layout = keyboard_get_layout(keyboard);
    KeyboardState state = keyboard_get_state(keyboard);

    // 1. Background (Only if not drawing dynamic overlay)
    if (!draw_dynamic) {
        Rectangle bg = {0, menu_offset, win_width, win_height - menu_offset};
        renderer_draw_rectangle(renderer, bg,
            color_with_opacity(scheme.background, opacity), KEYBOARD_CORNER_RADIUS);
    }

    for (int i = 0; i < key_count && i < layout->num_keys; i++) {
        KeyDef *key = &layout->keys[i];
        KeyVisualMetadata *meta = &key_metadata[i];
        Rectangle kb = key_bounds[i];

        // Modifier and pressed states are only relevant for the dynamic pass
        bool is_active_modifier = false;
        bool is_pressed = false;

        if (draw_dynamic) {
            if (key->flags & KEYFLAG_SHIFT)
                is_active_modifier = (state.current_layer == KEYBOARD_LAYER_SHIFT);
            else if (key->normal == XK_Caps_Lock)
                is_active_modifier = state.caps_lock;
            
            is_pressed = (state.pressed_key_index == i);
        }
        
        // Filtering: 
        // - Background pass (draw_dynamic=false): Draw EVERY key in its current label/state.
        // - Dynamic pass (draw_dynamic=true): Only draw keys that need a highlight (pressed or active modifier).
        if (draw_dynamic && !(is_pressed || is_active_modifier)) continue;

        // --- Key color selection (Using Enhanced Metadata) ---
        Color key_color;
        
        if (is_pressed) {
            key_color = scheme.key_pressed;
        } else if (is_active_modifier) {
            key_color = scheme.shift_active;
        } else if (meta->is_special) {
            key_color = scheme.key_special;
        } else if (meta->is_modifier) {
            key_color = scheme.key_modifier;
        } else if (meta->is_number) {
            key_color = scheme.key_number;
        } else if (meta->is_text) {
            key_color = scheme.key_text;
        } else {
            key_color = scheme.key_normal;
        }

        // 2. Key shadow
        Rectangle shadow_rect = {kb.x, kb.y + KEY_SHADOW_OFFSET, kb.width, kb.height};
        renderer_draw_rectangle(renderer, shadow_rect,
            color_with_opacity(scheme.key_shadow, opacity), KEY_CORNER_RADIUS);

        // 3. Key body
        renderer_draw_rectangle(renderer, kb,
            color_with_opacity(key_color, opacity), KEY_CORNER_RADIUS);

        // 4. Active modifier accent border
        if (is_active_modifier && !is_pressed) {
            renderer_draw_rectangle_outline(renderer, kb,
                color_with_opacity(scheme.accent, opacity * 0.8),
                2.0, KEY_CORNER_RADIUS);
        }

        // 5. Key label (Using Metadata)
        const char *label = keyboard_get_key_label(keyboard, i);
        if (label && label[0] != '\0') {
            FontSpec font = {
                font_family ? font_family : "Inter",
                meta->font_size, meta->bold, false
            };
            renderer_draw_text(renderer, label, kb, font, 
                              color_with_opacity(scheme.text_primary, opacity),
                              ALIGN_CENTER, VALIGN_CENTER);
        }
    }
}

void ui_render_draw_menu_bar(Renderer *renderer, int win_width,
                            double opacity, int font_size,
                            ColorScheme scheme) {
    if (!renderer) return;

    Rectangle menu_bar = {0, 0, win_width, MENU_BAR_HEIGHT};
    Color menu_color = color_with_opacity(scheme.background, opacity);
    renderer_draw_rectangle(renderer, menu_bar, menu_color, 0);

    // Divider line at bottom of menu
    Rectangle divider = {0, MENU_BAR_HEIGHT - 1, win_width, 1};
    Color div_color = color_with_opacity(scheme.key_modifier, opacity * 0.5);
    renderer_draw_rectangle(renderer, divider, div_color, 0);

    char menu_left[128];
    snprintf(menu_left, sizeof(menu_left), "[-] opacity [+]    [theme]");
    
    char menu_right[64];
    snprintf(menu_right, sizeof(menu_right), "[x]");

    FontSpec font = {
        "Inter", (int)(font_size * 0.75), false, false
    };
    Color text_color = color_with_opacity(scheme.text_secondary, opacity);
    
    // Left controls
    Rectangle left_bounds = {20, 0, win_width - 40, MENU_BAR_HEIGHT};
    renderer_draw_text(renderer, menu_left, left_bounds, font, text_color,
                      ALIGN_LEFT, VALIGN_CENTER);
                      
    // Right controls
    Rectangle right_bounds = {0, 0, win_width - 20, MENU_BAR_HEIGHT};
    renderer_draw_text(renderer, menu_right, right_bounds, font, text_color,
                      ALIGN_RIGHT, VALIGN_CENTER);
}

void ui_render_draw_drag_handle(Renderer *renderer, int win_width,
                               ColorScheme scheme, double opacity) {
    if (!renderer) return;
    
    double handle_w = 40;
    double handle_h = 4;
    Rectangle handle = {(win_width - handle_w)/2, (MENU_BAR_HEIGHT - handle_h)/2, (int)handle_w, (int)handle_h};
    
    renderer_draw_rectangle(renderer, handle, 
        color_with_opacity(scheme.drag_handle, opacity), 2);
}
