// layout.c - Distribución estilo Apple con escalonado natural
// Incluye Esc, Ctrl, Alt, Print en posiciones accesibles
#include "layout.h"
#include <X11/keysym.h>
#include <stddef.h>

static KeyDef apple_keys[] = {
    // ── Fila 1: Esc + Números + Del (escalonado base) ──
    {XK_Escape, 0, 0, "esc", false, 12},
    {XK_1, XK_exclam, XK_bar, "1", false, 10},
    {XK_2, XK_at, XK_quotedbl, "2", false, 10},
    {XK_3, XK_numbersign, XK_numbersign, "3", false, 10},
    {XK_4, XK_dollar, XK_asciitilde, "4", false, 10},
    {XK_5, XK_percent, XK_percent, "5", false, 10},
    {XK_6, XK_asciicircum, XK_ampersand, "6", false, 10},
    {XK_7, XK_ampersand, XK_slash, "7", false, 10},
    {XK_8, XK_asterisk, XK_parenleft, "8", false, 10},
    {XK_9, XK_parenleft, XK_parenright, "9", false, 10},
    {XK_0, XK_parenright, XK_equal, "0", false, 10},
    {XK_BackSpace, 0, 0, "del", false, 16},

    // ── Fila 2: Tab + QWERTY (tab más ancho = escalonado) ──
    {XK_Tab, 0, 0, "tab", true, 15},
    {XK_q, XK_Q, XK_backslash, "q", false, 10},
    {XK_w, XK_W, XK_grave, "w", false, 10},
    {XK_e, XK_E, XK_braceleft, "e", false, 10},
    {XK_r, XK_R, XK_braceright, "r", false, 10},
    {XK_t, XK_T, XK_bracketleft, "t", false, 10},
    {XK_y, XK_Y, XK_bracketright, "y", false, 10},
    {XK_u, XK_U, XK_minus, "u", false, 10},
    {XK_i, XK_I, XK_plus, "i", false, 10},
    {XK_o, XK_O, XK_underscore, "o", false, 10},
    {XK_p, XK_P, XK_question, "p", false, 10},

    // ── Fila 3: Caps + ASDF + Return (caps más ancho que tab) ──
    {XK_Caps_Lock, 0, 0, "caps", true, 18},
    {XK_a, XK_A, XK_less, "a", false, 10},
    {XK_s, XK_S, XK_greater, "s", false, 10},
    {XK_d, XK_D, XK_apostrophe, "d", false, 10},
    {XK_f, XK_F, XK_semicolon, "f", false, 10},
    {XK_g, XK_G, XK_colon, "g", false, 10},
    {XK_h, XK_H, XK_asterisk, "h", false, 10},
    {XK_j, XK_J, XK_exclam, "j", false, 10},
    {XK_k, XK_K, XK_question, "k", false, 10},
    {XK_l, XK_L, XK_bar, "l", false, 10},
    {0xf1, 0xd1, 0, "\xc3\xb1", false, 10},
    {XK_Return, 0, 0, "return", false, 18},

    // ── Fila 4: Shift + ZXCV (shift aún más ancho) ──
    {XK_Shift_L, 0, 0, "shift", true, 24},
    {XK_z, XK_Z, 0, "z", false, 10},
    {XK_x, XK_X, 0, "x", false, 10},
    {XK_c, XK_C, 0, "c", false, 10},
    {XK_v, XK_V, 0, "v", false, 10},
    {XK_b, XK_B, 0, "b", false, 10},
    {XK_n, XK_N, 0, "n", false, 10},
    {XK_m, XK_M, 0, "m", false, 10},
    {XK_comma, XK_less, 0, ",", false, 10},
    {XK_period, XK_greater, 0, ".", false, 10},
    {XK_Shift_R, 0, 0, "shift", false, 24},

    // ── Fila 5: Modificadores + Espacio + Print + Menú ──
    {0, 0, 0, "size", true, 7},
    {XK_Control_L, 0, 0, "ctrl", false, 7},
    {0, 0, 0, "?123", false, 7},
    {XK_Alt_L, 0, 0, "alt", false, 7},
    {XK_Super_L, 0, 0, "cmd", false, 9},
    {XK_space, 0, 0, " ", false, 38},
    {XK_Super_R, 0, 0, "cmd", false, 9},
    {XK_Alt_R, 0, 0, "alt", false, 7},
    {XK_Print, 0, 0, "prt", false, 7},
    {0, 0, 0, "menu", false, 7}
};

static Layout apple_layout = {
    .keys = apple_keys,
    .num_keys = sizeof(apple_keys) / sizeof(apple_keys[0])
};

Layout* layout_get_default() {
    return &apple_layout;
}
