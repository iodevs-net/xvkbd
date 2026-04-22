/*
 * 0-Board Virtual Keyboard
 * Copyright (c) 2026 Leonardo Vergara <leonardovergaramarin@gmail.com>
 * Licensed under the MIT License.
 */
#include "engine.h"
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <stdlib.h>
#include <stdio.h>

struct Engine {
    Display *display;
    bool use_xtest;
    int event_delay_ms;
    bool owns_display; // Whether we opened the display ourselves
};

#include <unistd.h>

Engine* engine_create(EngineConfig *config) {
    if (!config) return NULL;
    
    Engine *engine = malloc(sizeof(Engine));
    if (!engine) return NULL;
    
    // Always open a fresh display connection for the engine to avoid
    // interference with the UI window's display. This is more robust for XTest.
    engine->display = XOpenDisplay(NULL);
    if (!engine->display) {
        fprintf(stderr, "engine_create: failed to open independent X11 display\n");
        free(engine);
        return NULL;
    }
    
    engine->use_xtest = config->use_xtest;
    engine->event_delay_ms = config->event_delay_ms;
    engine->owns_display = true;
    
    return engine;
}

int engine_send_key(Engine *engine, KeySym keysym, bool pressed) {
    return engine_send_key_ex(engine, keysym, pressed, 0);
}

int engine_send_key_ex(Engine *engine, KeySym keysym, bool pressed, int modifiers) {
    if (!engine || !engine->display || keysym == 0) {
        return -1;
    }
    
    if (!engine->use_xtest) {
        fprintf(stderr, "engine_send_key: XTest required\n");
        return -1;
    }
    
    KeyCode kc = XKeysymToKeycode(engine->display, keysym);
    if (!kc) return -1;

    // Si se requiere Shift y no es la tecla Shift en sí misma
    bool need_shift = (modifiers & 1); // Simplificación: bit 0 es Shift
    KeyCode shift_kc = XKeysymToKeycode(engine->display, XK_Shift_L);

    if (pressed && need_shift && keysym != XK_Shift_L && keysym != XK_Shift_R) {
        XTestFakeKeyEvent(engine->display, shift_kc, True, 0);
    }

    XTestFakeKeyEvent(engine->display, kc, pressed, 0);

    if (!pressed && need_shift && keysym != XK_Shift_L && keysym != XK_Shift_R) {
        XTestFakeKeyEvent(engine->display, shift_kc, False, 0);
    }

    return 0;
}

void engine_flush(Engine *engine) {
    if (engine && engine->display) {
        XFlush(engine->display);
    }
}

void engine_destroy(Engine *engine) {
    if (!engine) return;
    
    if (engine->display && engine->owns_display) {
        XCloseDisplay(engine->display);
    }
    
    free(engine);
}