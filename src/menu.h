#ifndef XVKBD_MENU_H
#define XVKBD_MENU_H
#include <cairo/cairo.h>
#include "ui.h"

typedef struct { double r,g,b; const char *name; } ColorPreset;

#define N_PALETTE 8
extern const ColorPreset pal_bg[N_PALETTE];
extern const ColorPreset pal_key[N_PALETTE];
extern const ColorPreset pal_border[N_PALETTE];

void menu_apply_colors(UIState *s);
void menu_render(UIState *s, cairo_t *cr);
// Retorna true si el menú consumió el clic
bool menu_handle_click(UIState *s, int mx, int my);

#endif
