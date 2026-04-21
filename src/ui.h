// ui.h - Soporte para menú de personalización visual
#ifndef XVKBD_UI_H
#define XVKBD_UI_H

#include <X11/Xlib.h>
#include <stdbool.h>
#include "layout.h"

typedef enum { SIZE_S, SIZE_M, SIZE_L } UISize;
typedef enum { LAYER_NORMAL, LAYER_SHIFT, LAYER_SYMBOLS } KeyboardLayer;
typedef enum { MENU_CLOSED, MENU_MAIN, MENU_VISUAL } MenuState;

// ── Tema visual completo ──
typedef struct {
    double win_radius;       // empalme teclado (base, se escala)
    double key_radius;       // empalme teclas (base, se escala)
    double win_opacity;      // opacidad 5-100%
    double bg_r, bg_g, bg_b;           // fondo ventana
    double key1_r, key1_g, key1_b;     // teclas normales
    double key2_r, key2_g, key2_b;     // teclas modificadoras
    double key3_r, key3_g, key3_b;     // teclas activas
    double border_r, border_g, border_b; // contorno teclas
    double border_width;
    const char *font_family;
    int font_size;
    bool show_titlebar;
    // Valores escalados (internos)
    double s_win_r, s_key_r;
} Theme;

#define KEYFLAG_NORMAL   0
#define KEYFLAG_SHIFT    1
#define KEYFLAG_SYMBOLS  2
#define KEYFLAG_MODIFIER 4

typedef struct {
    int x, y, w, h;
    KeyDef *key;
    int flags;
} KeyRect;

typedef struct { int start, count, weight; } RowInfo;

// Zona clicable del menú
#define MAX_MENU_HITS 40
typedef struct { int x, y, w, h, id; } MenuHit;

typedef struct {
    Display *display;
    Window window;
    Visual *visual;
    int width, height;
    Theme theme;
    Layout *layout;
    KeyRect rects[128];
    int num_rects;
    RowInfo rows[10];
    int num_rows;
    KeyboardLayer current_layer;
    UISize current_size;
    Pixmap backbuffer;
    bool dirty;
    // Menú
    MenuState menu_state;
    int color_idx[5]; // bg, key1, key2, key3, border
    MenuHit menu_hits[MAX_MENU_HITS];
    int num_menu_hits;
} UIState;

void ui_init(UIState *state, Layout *layout);
void ui_loop(UIState *state);
void ui_set_size(UIState *state, UISize size);

#endif
