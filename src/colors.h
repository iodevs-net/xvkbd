#ifndef COLORS_H
#define COLORS_H

#include <stdbool.h>

// Color scheme definition
typedef struct {
    double background;
    double key_normal;
    double key_modifier;
    double text_light;    // for dark backgrounds
    double text_dark;     // for light backgrounds
} ColorScheme;

// Built-in color schemes
typedef enum {
    COLOR_SCHEME_DARK,
    COLOR_SCHEME_LIGHT,
    COLOR_SCHEME_BLUE_GRAY,
    COLOR_SCHEME_AMBER,
    NUM_COLOR_SCHEMES
} ColorSchemeId;

// Get a predefined color scheme
ColorScheme color_scheme_get(ColorSchemeId id);

// Helper to determine text color based on background brightness
bool should_use_light_text(double background_brightness);

#endif // COLORS_H