/*
 * 0-Board Virtual Keyboard
 * Copyright (c) 2026 Leonardo Vergara <leonardovergaramarin@gmail.com>
 * Licensed under the MIT License.
 */
#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

// Configuration structure for 0-board
typedef struct {
    // Window settings
    int window_width;
    int window_height;
    double window_opacity;
    bool window_borderless;
    bool window_skip_taskbar;
    
    // Keyboard settings
    int keyboard_size; // 0=small, 1=medium, 2=large
    int color_scheme;  // 0=light, 1=dark, 2=auto
    bool show_menu_bar;
    
    // Advanced settings
    bool double_buffering;
    bool lazy_font_loading;
} Config;

// Load default configuration
void config_load_defaults(Config *config);

// Load configuration from file
// Returns true on success, false on failure (falls back to defaults)
bool config_load_from_file(Config *config, const char *filename);

// Save configuration to file
// Returns true on success, false on failure
bool config_save_to_file(const Config *config, const char *filename);

// Get the default config file path (caller must free)
char* config_get_default_path(void);

#endif // CONFIG_H