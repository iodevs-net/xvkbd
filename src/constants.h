#ifndef CONSTANTS_H
#define CONSTANTS_H

// Keyboard constants
#define MAX_KEYS 128
#define DASH_HEIGHT 40
#define NUM_BUFFERED_SIZES 3
#define MAX_FONTS_TO_SCAN 512

// UI sizing
#define SCREEN_WIDTH_RATIO_SMALL 0.35
#define SCREEN_WIDTH_RATIO_MEDIUM 0.60
#define SCREEN_WIDTH_RATIO_LARGE 0.92
#define KEYBOARD_HEIGHT_RATIO 0.35  // height = width * ratio
#define KEY_PADDING_RATIO 0.015     // pad = width * ratio  
#define KEY_GAP_RATIO 0.004         // gap = width * ratio
#define FONT_SIZE_RATIO 0.022       // font_size = width * ratio + 2

// Menu constants
#define MENU_BAR_HEIGHT DASH_HEIGHT
#define MENU_OPACITY_STEP 0.1
#define MIN_OPACITY 0.1
#define MAX_OPACITY 1.0

// Rendering
#define KEYBOARD_CORNER_RADIUS 12.0
#define KEY_CORNER_RADIUS 5.0
#define CLEAR_ALPHA 0.0

// Color palette (grayscale values 0-1)
#define COLOR_NEAR_BLACK 0.05
#define COLOR_DARK_GRAY 0.12
#define COLOR_MEDIUM_DARK_GRAY 0.20
#define COLOR_MEDIUM_GRAY 0.35
#define COLOR_MID_GRAY 0.50
#define COLOR_LIGHT_GRAY 0.70
#define COLOR_NEAR_WHITE 0.90

// Layout row constants
#define MAX_ROWS 10

#endif // CONSTANTS_H