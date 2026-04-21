#include "keyboard.h"
#include "layout.h"
#include "x11_window.h"
#include "x11_cairo_bridge.h"
#include "config.h"
#include <sys/stat.h>
#include "font_manager.h"
#include "engine.h"
#include "ui.h"
#include "colors.h"
#include "constants.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>

// Simple error handler
static void error_exit(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(1);
}

int main() {
    // 0. Load configuration
    Config config;
    config_load_defaults(&config);
    
    DEBUG_INIT("debug.log");
    LOG_INFO("Starting 0-board (Clean Architecture)...");
    printf("Starting 0-board (Clean Architecture)...\n");
    
    // 1. Create layout
    Layout *layout = layout_get_default();
    if (!layout) {
        error_exit("Failed to get default layout");
    }
    
    layout_init(layout);
    
    // 2. Create keyboard
    Keyboard *keyboard = keyboard_create(layout);
    if (!keyboard) {
        error_exit("Failed to create keyboard");
    }
    
    // 3. Create font manager (with lazy loading)
    FontConfig font_config = {
        .preferred_family = NULL,
        .load_all_system_fonts = false, // Lazy loading
        .max_fonts_to_cache = 50        // Reasonable cache size
    };
    
    FontManager *font_manager = font_manager_create(&font_config);
    if (!font_manager) {
        error_exit("Failed to create font manager");
    }
    
    // 4. Calculate correct window size based on screen
    Display *tmp_dpy = XOpenDisplay(NULL);
    int screen_w = DisplayWidth(tmp_dpy, DefaultScreen(tmp_dpy));
    XCloseDisplay(tmp_dpy);
    
    double size_ratios[] = {SCREEN_WIDTH_RATIO_SMALL, SCREEN_WIDTH_RATIO_MEDIUM, SCREEN_WIDTH_RATIO_LARGE};
    int kb_width = screen_w * size_ratios[config.keyboard_size];
    int kb_height = kb_width * KEYBOARD_HEIGHT_RATIO;
    
    // 5. Create X11 window with correct size
    WindowConfig window_config = {
        .x = -1,
        .y = -1,
        .width = kb_width,
        .height = kb_height,
        .title = "0-board",
        .borderless = config.window_borderless,
        .override_redirect = false,
        .skip_taskbar = config.window_skip_taskbar,
        .opacity = config.window_opacity
    };
    
    X11Window *window = x11_window_create(&window_config);
    if (!window) {
        error_exit("Failed to create X11 window");
    }
    
    // 5. Create renderer for X11 window
    RendererConfig renderer_config = {
        .width = window_config.width,
        .height = window_config.height,
        .default_opacity = 1.0,
        .clear_color = {0.0, 0.0, 0.0, 0.0} // Transparent black
    };
    
    Renderer *renderer = renderer_create_for_x11_window(window, &renderer_config);
    if (!renderer) {
        error_exit("Failed to create renderer");
    }
    
    // 6. Create engine
    EngineConfig engine_config = {
        .display = x11_window_get_display(window),
        .use_xtest = true,
        .event_delay_ms = 0
    };
    
    Engine *engine = engine_create(&engine_config);
    if (!engine) {
        error_exit("Failed to create engine");
    }
    
    // 7. Create UI configuration
    UIConfig ui_config = {
        .title = "0-board",
        .initial_opacity = config.window_opacity,
        .show_menu_bar = config.show_menu_bar,
        .initial_size = config.keyboard_size
    };
    
    // 8. Create UI
    UI *ui = ui_create(&ui_config, keyboard, renderer, window, font_manager);
    if (!ui) {
        error_exit("Failed to create UI");
    }
    ui_set_engine(ui, engine);
    
    printf("0-board initialized successfully.\n");
    printf("Press Ctrl+C to exit.\n");
    
    // 9. Run UI main loop
    x11_window_show(window);
    ui_run(ui);
    
    printf("UI loop exited.\n");
    
    // 10. Cleanup
    ui_destroy(ui);
    renderer_destroy(renderer);
    engine_destroy(engine);
    x11_window_destroy(window);
    font_manager_destroy(font_manager);
    keyboard_destroy(keyboard);
    // Note: layout is static, don't free
    
    printf("0-board shutdown complete.\n");
    LOG_INFO("Shutting down 0-board...");
    DEBUG_CLEANUP();
    
    return 0;
}


