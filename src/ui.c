// ui.c - Motor Cairo con escalonado Apple y tamaños S/M/L
#include "ui.h"
#include "engine.h"
#include "menu.h"
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ── Traducción de KeySym a Label ──
typedef struct { KeySym ks; const char *label; } SymLabel;
static const SymLabel sym_table[] = {
    {XK_comma, ","}, {XK_period, "."}, {XK_slash, "/"}, {XK_backslash, "\\"},
    {XK_bracketleft, "["}, {XK_bracketright, "]"}, {XK_braceleft, "{"}, {XK_braceright, "}"},
    {XK_less, "<"}, {XK_greater, ">"}, {XK_quotedbl, "\""}, {XK_apostrophe, "'"},
    {XK_underscore, "_"}, {XK_equal, "="}, {XK_plus, "+"}, {XK_minus, "-"},
    {XK_asterisk, "*"}, {XK_ampersand, "&"}, {XK_percent, "%"}, {XK_dollar, "$"},
    {XK_numbersign, "#"}, {XK_at, "@"}, {XK_exclam, "!"}, {XK_question, "?"},
    {XK_bar, "|"}, {XK_asciitilde, "~"}, {XK_grave, "`"},
    {XK_semicolon, ";"}, {XK_colon, ":"},
    {0xf1, "\xc3\xb1"}, {0xd1, "\xc3\x91"},
    {0, NULL}
};

static const char* keysym_to_label(KeySym ks) {
    for (const SymLabel *s = sym_table; s->label; s++)
        if (s->ks == ks) return s->label;
    return NULL;
}

static const char* get_key_label(KeyDef *k, KeyboardLayer layer, int flags) {
    if (flags & KEYFLAG_MODIFIER) return k->label;
    KeySym ks = (layer == LAYER_SHIFT) ? k->shifted :
                (layer == LAYER_SYMBOLS) ? k->altgr : k->normal;
    if (ks == 0) ks = k->normal;
    const char *found = keysym_to_label(ks);
    if (found) return found;
    const char *name = XKeysymToString(ks);
    if (name && strlen(name) == 1) return name;
    return k->label;
}

// ── Geometría ──

static void draw_rounded_rect(cairo_t *cr, double x, double y,
                               double w, double h, double r) {
    if (r > w / 2) r = w / 2;
    if (r > h / 2) r = h / 2;
    cairo_new_sub_path(cr);
    cairo_arc(cr, x + w - r, y + r,     r, -1.5708, 0);
    cairo_arc(cr, x + w - r, y + h - r, r, 0,       1.5708);
    cairo_arc(cr, x + r,     y + h - r, r, 1.5708,  3.14159);
    cairo_arc(cr, x + r,     y + r,     r, 3.14159, 4.71239);
    cairo_close_path(cr);
}

// ── Cálculo de la grilla con escalonado ──

static void precompute_grid(UIState *state) {
    // Paso 1: Identificar filas
    state->num_rows = 0;
    for (int i = 0; i < state->layout->num_keys; i++) {
        if (state->layout->keys[i].new_row || i == 0) {
            RowInfo *ri = &state->rows[state->num_rows++];
            ri->start = i;
            ri->count = 0;
            ri->weight = 0;
        }
        RowInfo *ri = &state->rows[state->num_rows - 1];
        ri->count++;
        ri->weight += state->layout->keys[i].width_weight;
    }

    // Paso 2: Escalar márgenes y gaps al tamaño actual
    double scale = state->width / 1100.0;
    int pad = (int)(16 * scale);
    if (pad < 6) pad = 6;
    int gap = (int)(5 * scale);
    if (gap < 2) gap = 2;
    int key_radius = (int)(6 * scale);
    if (key_radius < 3) key_radius = 3;
    state->theme.key_radius = key_radius;
    state->theme.font_size = (int)(13.5 * scale);
    if (state->theme.font_size < 8) state->theme.font_size = 8;

    int total_w = state->width - 2 * pad;
    int total_h = state->height - 2 * pad;
    int row_h = (total_h - (state->num_rows - 1) * gap) / state->num_rows;

    // Paso 3: Posicionar teclas
    state->num_rects = 0;
    int cur_y = pad;

    for (int r = 0; r < state->num_rows; r++) {
        RowInfo *ri = &state->rows[r];
        int cur_x = pad;
        int usable_w = total_w - (gap * (ri->count - 1));

        for (int j = 0; j < ri->count; j++) {
            int idx = ri->start + j;
            KeyDef *k = &state->layout->keys[idx];
            int kw = (k->width_weight * usable_w) / ri->weight;

            KeyRect *kr = &state->rects[state->num_rects++];
            kr->x = cur_x;
            kr->y = cur_y;
            kr->w = kw;
            kr->h = row_h;
            kr->key = k;

            // Clasificar tipo de tecla
            kr->flags = KEYFLAG_NORMAL;
            if (strcmp(k->label, "shift") == 0)
                kr->flags = KEYFLAG_SHIFT | KEYFLAG_MODIFIER;
            else if (strcmp(k->label, "?123") == 0)
                kr->flags = KEYFLAG_SYMBOLS | KEYFLAG_MODIFIER;
            else if (strcmp(k->label, "size") == 0 ||
                     strcmp(k->label, "caps") == 0 ||
                     strcmp(k->label, "del") == 0 ||
                     strcmp(k->label, "return") == 0 ||
                     strcmp(k->label, "tab") == 0 ||
                     strcmp(k->label, "esc") == 0 ||
                     strcmp(k->label, "ctrl") == 0 ||
                     strcmp(k->label, "alt") == 0 ||
                     strcmp(k->label, "cmd") == 0 ||
                     strcmp(k->label, "prt") == 0 ||
                     strcmp(k->label, " ") == 0)
                kr->flags = KEYFLAG_MODIFIER;

            cur_x += kw + gap;
        }
        cur_y += row_h + gap;
    }
}

// ── Tamaños S / M / L ──

void ui_set_size(UIState *state, UISize size) {
    state->current_size = size;
    int screen_w = DisplayWidth(state->display, DefaultScreen(state->display));

    // S=25%, M=50%, L=92% (nunca se sale de pantalla)
    double ratios[] = {0.25, 0.50, 0.92};
    state->width = (int)(screen_w * ratios[size]);
    state->height = (int)(state->width / 3.2);

    // Limitar al borde de pantalla con margen
    if (state->width > screen_w - 20) {
        state->width = screen_w - 20;
        state->height = (int)(state->width / 3.2);
    }

    // Re-crear backbuffer
    if (state->backbuffer) XFreePixmap(state->display, state->backbuffer);
    XVisualInfo vinfo;
    XMatchVisualInfo(state->display, DefaultScreen(state->display),
                     32, TrueColor, &vinfo);
    state->backbuffer = XCreatePixmap(state->display, state->window,
                                      state->width, state->height, vinfo.depth);

    XResizeWindow(state->display, state->window, state->width, state->height);
    precompute_grid(state);
    state->dirty = true;
}

// ── Inicialización ──

void ui_init(UIState *state, Layout *layout) {
    state->display = XOpenDisplay(NULL);
    state->layout = layout;
    state->current_layer = LAYER_NORMAL;
    state->backbuffer = 0;

    state->theme.win_radius = 16;
    state->theme.key_radius = 6;
    state->theme.win_opacity = 0.94;
    state->theme.font_family = "Inter, Sans";
    state->theme.show_titlebar = false;

    Window root = DefaultRootWindow(state->display);
    XVisualInfo vinfo;
    XMatchVisualInfo(state->display, DefaultScreen(state->display),
                     32, TrueColor, &vinfo);
    state->visual = vinfo.visual;

    XSetWindowAttributes attr;
    attr.colormap = XCreateColormap(state->display, root,
                                     state->visual, AllocNone);
    attr.border_pixel = 0;
    attr.background_pixel = 0;
    attr.override_redirect = !state->theme.show_titlebar;
    attr.backing_store = WhenMapped;

    state->window = XCreateWindow(state->display, root,
        100, 100, 100, 100, 0,
        vinfo.depth, InputOutput, state->visual,
        CWColormap | CWBorderPixel | CWBackPixel |
        CWOverrideRedirect | CWBackingStore, &attr);

    // Iniciar en tamaño Medium
    ui_set_size(state, SIZE_M);

    XSelectInput(state->display, state->window,
        ExposureMask | ButtonPressMask | ButtonReleaseMask | Button1MotionMask);
    XMapWindow(state->display, state->window);
}

// ── Renderizado ──

static void render_to_backbuffer(UIState *state) {
    cairo_surface_t *surf = cairo_xlib_surface_create(
        state->display, state->backbuffer, state->visual,
        state->width, state->height);
    cairo_t *cr = cairo_create(surf);

    // Limpiar a transparente para evitar artefactos en esquinas
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    cairo_set_source_rgba(cr, 0, 0, 0, 0);
    cairo_paint(cr);

    // Fondo de la ventana con bordes redondeados
    cairo_set_source_rgba(cr, state->theme.bg_r, state->theme.bg_g, state->theme.bg_b, state->theme.win_opacity);
    draw_rounded_rect(cr, 0, 0, state->width, state->height,
                      state->theme.win_radius);
    cairo_fill(cr);

    // Configurar fuente
    cairo_select_font_face(cr, state->theme.font_family,
        CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, state->theme.font_size);
    cairo_set_operator(cr, CAIRO_OPERATOR_OVER);

    // Dibujar cada tecla
    for (int i = 0; i < state->num_rects; i++) {
        KeyRect *kr = &state->rects[i];

        // Color de fondo de la tecla
        bool active = ((kr->flags & KEYFLAG_SHIFT) &&
                       state->current_layer == LAYER_SHIFT) ||
                      ((kr->flags & KEYFLAG_SYMBOLS) &&
                       state->current_layer == LAYER_SYMBOLS);

        if (active)
            cairo_set_source_rgba(cr, state->theme.key3_r, state->theme.key3_g, state->theme.key3_b, 1.0);
        else if (kr->flags & KEYFLAG_MODIFIER)
            cairo_set_source_rgba(cr, state->theme.key2_r, state->theme.key2_g, state->theme.key2_b, 1.0);
        else
            cairo_set_source_rgba(cr, state->theme.key1_r, state->theme.key1_g, state->theme.key1_b, 1.0);

        draw_rounded_rect(cr, kr->x, kr->y, kr->w, kr->h,
                          state->theme.key_radius);
        cairo_fill(cr);

        // Contorno si aplica
        if (state->theme.border_width > 0) {
            cairo_set_source_rgb(cr, state->theme.border_r, state->theme.border_g, state->theme.border_b);
            cairo_set_line_width(cr, state->theme.border_width);
            draw_rounded_rect(cr, kr->x, kr->y, kr->w, kr->h, state->theme.key_radius);
            cairo_stroke(cr);
        }

        // Etiqueta de la tecla
        const char *label = get_key_label(kr->key,
                                           state->current_layer, kr->flags);
        cairo_set_source_rgb(cr, 0.95, 0.95, 0.95);
        cairo_text_extents_t ext;
        cairo_text_extents(cr, label, &ext);
        cairo_move_to(cr,
            kr->x + kr->w / 2.0 - ext.width / 2.0 - ext.x_bearing,
            kr->y + kr->h / 2.0 - ext.height / 2.0 - ext.y_bearing);
        cairo_show_text(cr, label);
    }

    // Renderizar menú si está abierto
    if (state->menu_state != MENU_CLOSED) {
        menu_render(state, cr);
    }

    cairo_destroy(cr);
    cairo_surface_destroy(surf);
    state->dirty = false;
}

// ── Bucle de eventos ──

void ui_loop(UIState *state) {
    XEvent ev;
    int drag_x = 0, drag_y = 0;
    bool dragging = false;

    while (1) {
        XNextEvent(state->display, &ev);

        // Repintar al exponer
        if (ev.type == Expose && ev.xexpose.count == 0) {
            if (state->dirty) render_to_backbuffer(state);
            GC gc = XCreateGC(state->display, state->window, 0, NULL);
            XCopyArea(state->display, state->backbuffer, state->window, gc,
                      0, 0, state->width, state->height, 0, 0);
            XFreeGC(state->display, gc);
        }

        // Clic
        if (ev.type == ButtonPress) {
            // Botón derecho: ciclar tamaños
            if (ev.xbutton.button == 3) {
                ui_set_size(state, (state->current_size + 1) % 3);
            }
            // Botón izquierdo: tecla o arrastre
            else if (ev.xbutton.button == 1) {
                int mx = ev.xbutton.x, my = ev.xbutton.y;

                // Si el menú está abierto, delegar clics
                if (state->menu_state != MENU_CLOSED) {
                    if (menu_handle_click(state, mx, my)) {
                        if (state->dirty) {
                            render_to_backbuffer(state);
                            XClearWindow(state->display, state->window);
                        }
                        continue;
                    }
                }

                bool on_key = false;
                for (int i = 0; i < state->num_rects; i++) {
                    KeyRect *kr = &state->rects[i];
                    if (mx >= kr->x && mx <= kr->x + kr->w &&
                        my >= kr->y && my <= kr->y + kr->h) {
                        on_key = true;

                        // Tecla de tamaño
                        if (strcmp(kr->key->label, "size") == 0) {
                            ui_set_size(state,
                                (state->current_size + 1) % 3);
                        }
                        // Shift
                        else if (kr->flags & KEYFLAG_SHIFT) {
                            state->current_layer =
                                (state->current_layer == LAYER_SHIFT)
                                ? LAYER_NORMAL : LAYER_SHIFT;
                            state->dirty = true;
                        }
                        // Símbolos
                        else if (kr->flags & KEYFLAG_SYMBOLS) {
                            state->current_layer =
                                (state->current_layer == LAYER_SYMBOLS)
                                ? LAYER_NORMAL : LAYER_SYMBOLS;
                            state->dirty = true;
                        }
                        // Tecla de menú
                        else if (strcmp(kr->key->label, "menu") == 0) {
                            state->menu_state = (state->menu_state == MENU_CLOSED) ? MENU_MAIN : MENU_CLOSED;
                            state->dirty = true;
                        }
                        // Tecla normal
                        else {
                            KeySym ks;
                            if (state->current_layer == LAYER_SHIFT)
                                ks = kr->key->shifted;
                            else if (state->current_layer == LAYER_SYMBOLS)
                                ks = kr->key->altgr;
                            else
                                ks = kr->key->normal;
                            if (ks != 0) {
                                engine_send_key(ks, true);
                                engine_send_key(ks, false);
                            }
                            if (state->current_layer == LAYER_SHIFT) {
                                state->current_layer = LAYER_NORMAL;
                                state->dirty = true;
                            }
                        }

                        if (state->dirty) {
                            render_to_backbuffer(state);
                            XClearWindow(state->display, state->window);
                        }
                        break;
                    }
                }
                if (!on_key) {
                    dragging = true;
                    drag_x = mx;
                    drag_y = my;
                }
            }
        }

        // Soltar botón
        if (ev.type == ButtonRelease && ev.xbutton.button == 1)
            dragging = false;

        // Arrastre de ventana
        if (ev.type == MotionNotify && dragging)
            XMoveWindow(state->display, state->window,
                ev.xmotion.x_root - drag_x, ev.xmotion.y_root - drag_y);
    }
}
