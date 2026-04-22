/*
 * 0-Board Virtual Keyboard
 * Copyright (c) 2026 Leonardo Vergara <leonardovergaramarin@gmail.com>
 * Licensed under the MIT License.
 */
#ifndef COLORS_H
#define COLORS_H

#include "renderer.h"

// Color scheme definition — real RGBA, not grayscale
typedef struct {
    Color background;
    Color key_normal;
    Color key_modifier;
    Color key_special;
    Color text_primary;
    Color text_secondary;
    Color key_pressed;
    Color accent;
    Color drag_handle;
    Color key_shadow;
} ColorScheme;

typedef enum {
    COLOR_SCHEME_SPACE_GRAY,
    COLOR_SCHEME_SILVER,
    NUM_COLOR_SCHEMES
} ColorSchemeId;

ColorScheme color_scheme_get(int id);

#endif // COLORS_H