#include "ui_events.h"
#include "ui_internal.h"
#include "constants.h"
#include "engine.h"
#include "colors.h"
#include "debug.h"
#include <string.h>

static void handle_button_press(UI *ui, WindowEvent *event) {
    int mx = event->x, my = event->y;
    bool hit = false;
    
    // 1. Menu bar interactions
    if (ui->menu_visible && my < MENU_BAR_HEIGHT) {
        hit = true;
        Display *dpy = x11_window_get_display(ui->window);
        int screen_w = DisplayWidth(dpy, DefaultScreen(dpy));
        double ratios[] = {SCREEN_WIDTH_RATIO_SMALL, SCREEN_WIDTH_RATIO_MEDIUM, SCREEN_WIDTH_RATIO_LARGE};
        int w = screen_w * ratios[ui->size_index];
        
        if (mx < w/10) ui_set_opacity(ui, ui->opacity - MENU_OPACITY_STEP);
        else if (mx < w/4) ui_set_opacity(ui, ui->opacity + MENU_OPACITY_STEP);
        else if (mx < 2*w/4) ui_set_color_scheme(ui, (ui->color_scheme_index + 1) % NUM_COLOR_SCHEMES);
        else if (mx < 3*w/4) ui_set_font_family(ui, "sans-serif");
        else ui->should_close = true;
    } 
    // 2. Keyboard keys interactions
    else {
        for (int i = 0; i < ui->key_count; i++) {
            Rectangle *kb = &ui->key_bounds[i];
            if (mx >= kb->x && mx <= kb->x + kb->width && 
                my >= kb->y && my <= kb->y + kb->height) {
                hit = true;
                const char *label = keyboard_get_key_label(ui->keyboard, i);
                
                if (strcmp(label, "menu") == 0) {
                    LOG_DEBUG("Menu toggled via key");
                    ui->menu_visible = !ui->menu_visible;
                    ui_calculate_layout(ui);
                } else if (strcmp(label, "size") == 0) {
                    LOG_DEBUG("Size cycled via key (index: %d)", ui->size_index);
                    ui_set_size_index(ui, (ui->size_index + 1) % 3);
                } else if (ui->engine) {
                    KeySym sym = keyboard_get_keysym(ui->keyboard, i);
                    KeyboardState state = keyboard_get_state(ui->keyboard);
                    
                    LOG_DEBUG("EVENT: Click on key %d ('%s'), current_layer=%d, sym=0x%lx", 
                              i, label, state.current_layer, sym);
                    
                    if (sym != 0) {
                        keyboard_press_key(ui->keyboard, i);
                        KeyboardState new_state = keyboard_get_state(ui->keyboard);
                        LOG_DEBUG("STATE CHANGE: layer %d -> %d", state.current_layer, new_state.current_layer);
                        
                        int effective_layer = keyboard_get_effective_layer(ui->keyboard, i);
                        int modifiers = (effective_layer == 1) ? 1 : 0; // 1 is LAYER_SHIFT
                        engine_send_key_ex(ui->engine, sym, true, modifiers);
                        engine_send_key_ex(ui->engine, sym, false, modifiers);
                        engine_flush(ui->engine);
                        
                        keyboard_notify_key_sent(ui->keyboard, i);
                    }
                }
                break;
            }
        }
    }
    
    // 3. Window dragging if no hit
    if (!hit) {
        ui->dragging = true;
        ui->drag_start_root_x = event->root_x;
        ui->drag_start_root_y = event->root_y;
        Window child;
        XTranslateCoordinates(x11_window_get_display(ui->window),
            x11_window_get_id(ui->window),
            DefaultRootWindow(x11_window_get_display(ui->window)),
            0, 0, &ui->win_start_x, &ui->win_start_y, &child);
    }
}

void ui_event_callback(X11Window *window, WindowEvent *event, void *user_data) {
    UI *ui = (UI *)user_data;
    if (!ui) return;
    (void)window;

    switch (event->type) {
        case WINDOW_EVENT_CLOSE:
            ui->should_close = true;
            break;
        case WINDOW_EVENT_BUTTON_PRESS:
            handle_button_press(ui, event);
            break;
        case WINDOW_EVENT_BUTTON_RELEASE:
            ui->dragging = false;
            break;
        case WINDOW_EVENT_MOTION:
            if (ui->dragging) {
                int dx = event->root_x - ui->drag_start_root_x;
                int dy = event->root_y - ui->drag_start_root_y;
                x11_window_move(ui->window, ui->win_start_x + dx, ui->win_start_y + dy);
            }
            break;
        case WINDOW_EVENT_EXPOSE:
            ui->dirty = true;
            break;
        case WINDOW_EVENT_RESIZE:
            if (event->width != ui->current_width || event->height != ui->current_height) {
                LOG_DEBUG("Window resized by WM: %dx%d", event->width, event->height);
                ui->current_width = event->width;
                ui->current_height = event->height;
                ui_calculate_layout(ui);
            }
            break;
        default:
            break;
    }
}
