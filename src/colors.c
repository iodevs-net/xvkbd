#include "colors.h"
#include "constants.h"

ColorScheme color_scheme_get(ColorSchemeId id) {
    switch (id) {
        case COLOR_SCHEME_DARK:
            return (ColorScheme){
                .background = COLOR_NEAR_BLACK,
                .key_normal = COLOR_DARK_GRAY,
                .key_modifier = COLOR_MEDIUM_DARK_GRAY,
                .text_light = 1.0,
                .text_dark = 0.0
            };
            
        case COLOR_SCHEME_LIGHT:
            return (ColorScheme){
                .background = COLOR_NEAR_WHITE,
                .key_normal = COLOR_LIGHT_GRAY,
                .key_modifier = COLOR_MID_GRAY,
                .text_light = 1.0,
                .text_dark = 0.0
            };
            
        case COLOR_SCHEME_BLUE_GRAY:
            return (ColorScheme){
                .background = 0.15,
                .key_normal = 0.25,
                .key_modifier = 0.35,
                .text_light = 1.0,
                .text_dark = 0.0
            };
            
        case COLOR_SCHEME_AMBER:
            return (ColorScheme){
                .background = 0.95,
                .key_normal = 0.85,
                .key_modifier = 0.75,
                .text_light = 1.0,
                .text_dark = 0.0
            };
            
        default:
            return color_scheme_get(COLOR_SCHEME_DARK);
    }
}

bool should_use_light_text(double background_brightness) {
    // Use light text on dark backgrounds (brightness < 0.5)
    return background_brightness < 0.5;
}