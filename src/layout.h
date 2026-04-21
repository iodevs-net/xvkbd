#ifndef XVKBD_LAYOUT_H
#define XVKBD_LAYOUT_H

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdbool.h>

typedef struct {
    KeySym normal;
    KeySym shifted;
    KeySym altgr;
    const char *label;
    bool new_row;
    int width_weight;
} KeyDef;

typedef struct {
    const char *name;
    KeyDef *keys;
    int num_keys;
} Layout;

Layout* layout_get_default();

#endif
