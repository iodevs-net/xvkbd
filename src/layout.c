#include "layout.h"
#include <X11/keysym.h>
#include <string.h>

static KeyDef default_keys[] = {
    // Fila 1: Números y Escape
    {XK_Escape, 0, 0, "esc", NULL, true, 8, 0},
    {XK_1, XK_exclam, 0, "1", "!", false, 8, 0}, {XK_2, XK_at, 0, "2", "@", false, 8, 0}, {XK_3, XK_numbersign, 0, "3", "#", false, 8, 0},
    {XK_4, XK_dollar, 0, "4", "$", false, 8, 0}, {XK_5, XK_percent, 0, "5", "%", false, 8, 0}, {XK_6, XK_asciicircum, 0, "6", "^", false, 8, 0},
    {XK_7, XK_ampersand, 0, "7", "&", false, 8, 0}, {XK_8, XK_asterisk, 0, "8", "*", false, 8, 0}, {XK_9, XK_parenleft, 0, "9", "(", false, 8, 0},
    {XK_0, XK_parenright, 0, "0", ")", false, 8, 0}, {XK_BackSpace, 0, 0, "del", NULL, false, 12, 0},

    // Fila 2: QWERTY
    {XK_Tab, 0, 0, "tab", NULL, true, 12, 0},
    {XK_q, XK_Q, 0, "q", "Q", false, 8, 0}, {XK_w, XK_W, 0, "w", "W", false, 8, 0}, {XK_e, XK_E, 0, "e", "E", false, 8, 0},
    {XK_r, XK_R, 0, "r", "R", false, 8, 0}, {XK_t, XK_T, 0, "t", "T", false, 8, 0}, {XK_y, XK_Y, 0, "y", "Y", false, 8, 0},
    {XK_u, XK_U, 0, "u", "U", false, 8, 0}, {XK_i, XK_I, 0, "i", "I", false, 8, 0}, {XK_o, XK_O, 0, "o", "O", false, 8, 0},
    {XK_p, XK_P, 0, "p", "P", false, 8, 0}, {XK_Return, 0, 0, "ret", NULL, false, 12, 0},

    // Fila 3: ASDF
    {XK_Caps_Lock, 0, 0, "caps", NULL, true, 14, 0},
    {XK_a, XK_A, 0, "a", "A", false, 8, 0}, {XK_s, XK_S, 0, "s", "S", false, 8, 0}, {XK_d, XK_D, 0, "d", "D", false, 8, 0},
    {XK_f, XK_F, 0, "f", "F", false, 8, 0}, {XK_g, XK_G, 0, "g", "G", false, 8, 0}, {XK_h, XK_H, 0, "h", "H", false, 8, 0},
    {XK_j, XK_J, 0, "j", "J", false, 8, 0}, {XK_k, XK_K, 0, "k", "K", false, 8, 0}, {XK_l, XK_L, 0, "l", "L", false, 8, 0},
    {0xf1, 0xd1, 0, "ñ", "Ñ", false, 8, 0}, {XK_apostrophe, XK_quotedbl, 0, "'", "\"", false, 10, 0},

    // Fila 4: ZXCV
    {XK_Shift_L, 0, 0, "shift", NULL, true, 18, 0},
    {XK_z, XK_Z, 0, "z", "Z", false, 8, 0}, {XK_x, XK_X, 0, "x", "X", false, 8, 0}, {XK_c, XK_C, 0, "c", "C", false, 8, 0},
    {XK_v, XK_V, 0, "v", "V", false, 8, 0}, {XK_b, XK_B, 0, "b", "B", false, 8, 0}, {XK_n, XK_N, 0, "n", "N", false, 8, 0},
    {XK_m, XK_M, 0, "m", "M", false, 8, 0}, {XK_comma, XK_less, 0, ",", "<", false, 8, 0}, {XK_period, XK_greater, 0, ".", ">", false, 8, 0},
    {XK_Shift_R, 0, 0, "shift", NULL, false, 18, 0},

    // Fila 5: Bottom
    {0, 0, 0, "menu", NULL, true, 10, 0}, {XK_Control_L, 0, 0, "ctrl", NULL, false, 10, 0}, {XK_Alt_L, 0, 0, "alt", NULL, false, 10, 0},
    {XK_space, 0, 0, " ", NULL, false, 40, 0}, {XK_Alt_R, 0, 0, "alt", NULL, false, 10, 0}, {0, 0, 0, "size", NULL, false, 10, 0}
};

static Layout main_layout = { "Full QWERTY", default_keys, sizeof(default_keys)/sizeof(default_keys[0]) };

Layout* layout_get_default() { return &main_layout; }

void layout_init(Layout *l) {
    for (int i = 0; i < l->num_keys; i++) {
        KeyDef *k = &l->keys[i];
        k->flags = KEYFLAG_NORMAL;
        if (!k->label) continue;
        if (strcmp(k->label, "shift") == 0) k->flags = KEYFLAG_SHIFT | KEYFLAG_MODIFIER;
        else if (strcmp(k->label, "menu") == 0 || strcmp(k->label, "size") == 0 || 
                 strcmp(k->label, "ctrl") == 0 || strcmp(k->label, "alt") == 0 || 
                 strcmp(k->label, "tab") == 0 || strcmp(k->label, "esc") == 0 || 
                 strcmp(k->label, "del") == 0 || strcmp(k->label, "ret") == 0 ||
                 strcmp(k->label, "caps") == 0)
            k->flags = KEYFLAG_MODIFIER;
    }
}
