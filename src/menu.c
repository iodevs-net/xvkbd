// menu.c - Panel de personalización visual
#include "menu.h"
#include <string.h>
#include <stdio.h>

const ColorPreset pal_bg[N_PALETTE] = {
    {0.10,0.10,0.10,"Carbón"},{0.06,0.07,0.13,"Azul noche"},
    {0.13,0.06,0.06,"Vino"},{0.05,0.10,0.07,"Bosque"},
    {0.09,0.05,0.12,"Púrpura"},{0.03,0.03,0.03,"Negro"},
    {0.18,0.18,0.18,"Gris"},{0.10,0.12,0.16,"Acero"}
};
const ColorPreset pal_key[N_PALETTE] = {
    {0.22,0.22,0.22,"Gris"},{0.18,0.20,0.28,"Azul"},
    {0.28,0.18,0.18,"Rojo"},{0.18,0.25,0.20,"Verde"},
    {0.24,0.18,0.26,"Púrpura"},{0.30,0.30,0.30,"Claro"},
    {0.16,0.16,0.16,"Oscuro"},{0.26,0.22,0.18,"Marrón"}
};
const ColorPreset pal_border[N_PALETTE] = {
    {0.00,0.00,0.00,"Ninguno"},{0.30,0.30,0.30,"Gris"},
    {0.25,0.30,0.40,"Azul"},{0.40,0.40,0.40,"Claro"},
    {0.50,0.40,0.20,"Dorado"},{0.30,0.20,0.20,"Rojo"},
    {0.20,0.30,0.25,"Verde"},{0.35,0.25,0.40,"Púrpura"}
};

void menu_apply_colors(UIState *s) {
    const ColorPreset *c;
    c = &pal_bg[s->color_idx[0]];
    s->theme.bg_r=c->r; s->theme.bg_g=c->g; s->theme.bg_b=c->b;
    c = &pal_key[s->color_idx[1]];
    s->theme.key1_r=c->r; s->theme.key1_g=c->g; s->theme.key1_b=c->b;
    c = &pal_key[s->color_idx[2]];
    s->theme.key2_r=c->r; s->theme.key2_g=c->g; s->theme.key2_b=c->b;
    c = &pal_key[s->color_idx[3]];
    s->theme.key3_r=c->r; s->theme.key3_g=c->g; s->theme.key3_b=c->b;
    c = &pal_border[s->color_idx[4]];
    s->theme.border_r=c->r; s->theme.border_g=c->g; s->theme.border_b=c->b;
    s->theme.border_width = (s->color_idx[4] == 0) ? 0.0 : 1.0;
}

// Helpers de dibujo
static void drr(cairo_t *cr, double x,double y,double w,double h,double r){
    if(r>w/2)r=w/2; if(r>h/2)r=h/2;
    cairo_new_sub_path(cr);
    cairo_arc(cr,x+w-r,y+r,r,-1.5708,0);
    cairo_arc(cr,x+w-r,y+h-r,r,0,1.5708);
    cairo_arc(cr,x+r,y+h-r,r,1.5708,3.14159);
    cairo_arc(cr,x+r,y+r,r,3.14159,4.71239);
    cairo_close_path(cr);
}

static void add_hit(UIState *s, int x,int y,int w,int h,int id){
    if(s->num_menu_hits < MAX_MENU_HITS){
        MenuHit *m = &s->menu_hits[s->num_menu_hits++];
        m->x=x; m->y=y; m->w=w; m->h=h; m->id=id;
    }
}

// Renderiza fila con swatch de color + flechas
static void draw_color_row(UIState *s, cairo_t *cr, int x, int y,
                           int w, int rh, const char *label,
                           const ColorPreset *pal, int idx, int field_id) {
    double fs = s->theme.font_size * 0.85;
    cairo_set_font_size(cr, fs);
    cairo_set_source_rgb(cr, 0.85,0.85,0.85);
    cairo_move_to(cr, x+8, y+rh/2.0+fs/3.0);
    cairo_show_text(cr, label);

    // Swatch
    int sw=rh-8, sx=x+w/2+20, sy=y+4;
    cairo_set_source_rgb(cr, pal[idx].r, pal[idx].g, pal[idx].b);
    drr(cr,sx,sy,sw,sw,3); cairo_fill(cr);
    cairo_set_source_rgba(cr,0.5,0.5,0.5,0.6);
    drr(cr,sx,sy,sw,sw,3); cairo_set_line_width(cr,1); cairo_stroke(cr);

    // Nombre del color
    cairo_set_source_rgb(cr, 0.6,0.6,0.6);
    cairo_set_font_size(cr, fs*0.8);
    cairo_move_to(cr, sx+sw+8, y+rh/2.0+fs/3.0);
    cairo_show_text(cr, pal[idx].name);

    // Flecha izquierda
    int bw=rh-4, bx=x+w-bw*2-12, by=y+2;
    cairo_set_source_rgba(cr,0.3,0.3,0.3,0.9);
    drr(cr,bx,by,bw,bw,4); cairo_fill(cr);
    cairo_set_source_rgb(cr,0.8,0.8,0.8);
    cairo_set_font_size(cr,fs);
    cairo_move_to(cr,bx+bw/2.0-4,by+bw/2.0+fs/3.0);
    cairo_show_text(cr,"\xe2\x97\x84"); // ◄
    add_hit(s, bx,by,bw,bw, 100+field_id*2);

    // Flecha derecha
    bx += bw+4;
    cairo_set_source_rgba(cr,0.3,0.3,0.3,0.9);
    drr(cr,bx,by,bw,bw,4); cairo_fill(cr);
    cairo_set_source_rgb(cr,0.8,0.8,0.8);
    cairo_move_to(cr,bx+bw/2.0-4,by+bw/2.0+fs/3.0);
    cairo_show_text(cr,"\xe2\x96\xba"); // ►
    add_hit(s, bx,by,bw,bw, 101+field_id*2);
}

// Renderiza fila con slider numérico + flechas
static void draw_slider_row(UIState *s, cairo_t *cr, int x, int y,
                            int w, int rh, const char *label,
                            double val, const char *suffix, int field_id) {
    double fs = s->theme.font_size * 0.85;
    cairo_set_font_size(cr, fs);
    cairo_set_source_rgb(cr, 0.85,0.85,0.85);
    cairo_move_to(cr, x+8, y+rh/2.0+fs/3.0);
    cairo_show_text(cr, label);

    // Valor
    char buf[32];
    snprintf(buf,sizeof(buf),"%.0f%s", val, suffix);
    cairo_set_source_rgb(cr,0.9,0.8,0.4);
    cairo_move_to(cr, x+w/2+20, y+rh/2.0+fs/3.0);
    cairo_show_text(cr, buf);

    // Flechas
    int bw=rh-4, bx=x+w-bw*2-12, by=y+2;
    cairo_set_source_rgba(cr,0.3,0.3,0.3,0.9);
    drr(cr,bx,by,bw,bw,4); cairo_fill(cr);
    cairo_set_source_rgb(cr,0.8,0.8,0.8);
    cairo_set_font_size(cr,fs);
    cairo_move_to(cr,bx+bw/2.0-4,by+bw/2.0+fs/3.0);
    cairo_show_text(cr,"\xe2\x97\x84");
    add_hit(s, bx,by,bw,bw, 100+field_id*2);
    bx += bw+4;
    cairo_set_source_rgba(cr,0.3,0.3,0.3,0.9);
    drr(cr,bx,by,bw,bw,4); cairo_fill(cr);
    cairo_set_source_rgb(cr,0.8,0.8,0.8);
    cairo_move_to(cr,bx+bw/2.0-4,by+bw/2.0+fs/3.0);
    cairo_show_text(cr,"\xe2\x96\xba");
    add_hit(s, bx,by,bw,bw, 101+field_id*2);
}

void menu_render(UIState *s, cairo_t *cr) {
    s->num_menu_hits = 0;
    int W=s->width, H=s->height;
    // Overlay oscuro
    cairo_set_source_rgba(cr,0,0,0,0.55);
    cairo_rectangle(cr,0,0,W,H); cairo_fill(cr);
    // Panel
    int pw=W*0.82, ph=H*0.88;
    int px=(W-pw)/2, py=(H-ph)/2;
    cairo_set_source_rgba(cr,0.12,0.12,0.14,0.97);
    drr(cr,px,py,pw,ph,12); cairo_fill(cr);
    cairo_set_source_rgba(cr,0.3,0.3,0.35,0.5);
    drr(cr,px,py,pw,ph,12); cairo_set_line_width(cr,1.5); cairo_stroke(cr);

    double fs = s->theme.font_size;

    if (s->menu_state == MENU_MAIN) {
        // Título
        cairo_set_source_rgb(cr,0.9,0.9,0.9);
        cairo_set_font_size(cr, fs*1.2);
        cairo_move_to(cr, px+20, py+ph*0.18);
        cairo_show_text(cr, "\xe2\x9a\x99 Configuración"); // ⚙

        // Opción 1: Personalización visual
        int iy = py+ph*0.32, ih=ph*0.22;
        cairo_set_source_rgba(cr,0.22,0.22,0.25,0.8);
        drr(cr,px+16,iy,pw-32,ih,8); cairo_fill(cr);
        cairo_set_source_rgb(cr,0.85,0.85,0.85);
        cairo_set_font_size(cr,fs);
        cairo_move_to(cr,px+32,iy+ih/2.0+fs/3.0);
        cairo_show_text(cr,"\xf0\x9f\x8e\xa8  Personalización visual"); // 🎨
        add_hit(s, px+16,iy,pw-32,ih, 1);

        // Opción 2: Cerrar menú
        iy = py+ph*0.60; ih=ph*0.22;
        cairo_set_source_rgba(cr,0.22,0.22,0.25,0.8);
        drr(cr,px+16,iy,pw-32,ih,8); cairo_fill(cr);
        cairo_set_source_rgb(cr,0.7,0.7,0.7);
        cairo_set_font_size(cr,fs);
        cairo_move_to(cr,px+32,iy+ih/2.0+fs/3.0);
        cairo_show_text(cr,"\xe2\x9c\x95  Cerrar menú"); // ✕
        add_hit(s, px+16,iy,pw-32,ih, 2);

    } else if (s->menu_state == MENU_VISUAL) {
        // Título
        cairo_set_source_rgb(cr,0.9,0.8,0.4);
        cairo_set_font_size(cr, fs*1.0);
        cairo_move_to(cr, px+20, py+20+fs);
        cairo_show_text(cr, "\xf0\x9f\x8e\xa8 Personalización Visual");

        int rh = (ph-50) / 9;
        int ry = py+38+fs;
        int cw = pw-32;

        draw_color_row(s,cr,px+16,ry,cw,rh,"Fondo teclado",
                       pal_bg, s->color_idx[0], 0);
        ry+=rh;
        draw_color_row(s,cr,px+16,ry,cw,rh,"Color teclas 1",
                       pal_key, s->color_idx[1], 1);
        ry+=rh;
        draw_color_row(s,cr,px+16,ry,cw,rh,"Color teclas 2",
                       pal_key, s->color_idx[2], 2);
        ry+=rh;
        draw_color_row(s,cr,px+16,ry,cw,rh,"Color teclas 3",
                       pal_key, s->color_idx[3], 3);
        ry+=rh;
        draw_color_row(s,cr,px+16,ry,cw,rh,"Contorno teclas",
                       pal_border, s->color_idx[4], 4);
        ry+=rh;
        draw_slider_row(s,cr,px+16,ry,cw,rh,"Empalme teclado",
                        s->theme.win_radius, "", 5);
        ry+=rh;
        draw_slider_row(s,cr,px+16,ry,cw,rh,"Empalme teclas",
                        s->theme.key_radius, "", 6);
        ry+=rh;
        draw_slider_row(s,cr,px+16,ry,cw,rh,"Opacidad",
                        s->theme.win_opacity*100.0, "%", 7);
        ry+=rh;
        // Botón volver
        cairo_set_source_rgba(cr,0.25,0.22,0.20,0.9);
        drr(cr,px+16,ry,cw,rh-4,6); cairo_fill(cr);
        cairo_set_source_rgb(cr,0.85,0.85,0.85);
        cairo_set_font_size(cr, fs*0.85);
        cairo_move_to(cr,px+30,ry+rh/2.0+fs*0.3);
        cairo_show_text(cr,"\xe2\x86\x90 Volver"); // ←
        add_hit(s, px+16,ry,cw,rh-4, 99);
    }
}

bool menu_handle_click(UIState *s, int mx, int my) {
    if (s->menu_state == MENU_CLOSED) return false;
    for (int i=0; i<s->num_menu_hits; i++) {
        MenuHit *h = &s->menu_hits[i];
        if (mx>=h->x && mx<=h->x+h->w && my>=h->y && my<=h->y+h->h) {
            int id = h->id;
            if (s->menu_state == MENU_MAIN) {
                if (id==1) s->menu_state = MENU_VISUAL;
                if (id==2) s->menu_state = MENU_CLOSED;
            } else if (s->menu_state == MENU_VISUAL) {
                if (id==99) { s->menu_state = MENU_MAIN; }
                // Colores: campos 0-4
                else if (id>=100 && id<=109) {
                    int field=(id-100)/2; bool right=(id%2==1);
                    if (field<=4) {
                        int *ci = &s->color_idx[field];
                        *ci = right ? (*ci+1)%N_PALETTE
                                    : (*ci+N_PALETTE-1)%N_PALETTE;
                        menu_apply_colors(s);
                    }
                }
                // Sliders: campos 5-7
                else if (id>=110 && id<=115) {
                    int field=(id-110)/2+5; bool right=(id%2==1);
                    double step;
                    if (field==5) { // empalme teclado
                        step=2; s->theme.win_radius += right?step:-step;
                        if(s->theme.win_radius<0) s->theme.win_radius=0;
                        if(s->theme.win_radius>30) s->theme.win_radius=30;
                    } else if (field==6) { // empalme teclas
                        step=1; s->theme.key_radius += right?step:-step;
                        if(s->theme.key_radius<0) s->theme.key_radius=0;
                        if(s->theme.key_radius>15) s->theme.key_radius=15;
                    } else if (field==7) { // opacidad
                        step=0.05;
                        s->theme.win_opacity += right?step:-step;
                        if(s->theme.win_opacity<0.05)s->theme.win_opacity=0.05;
                        if(s->theme.win_opacity>1.0) s->theme.win_opacity=1.0;
                    }
                }
            }
            s->dirty = true;
            return true;
        }
    }
    // Clic fuera del panel cierra el menú
    s->menu_state = MENU_CLOSED;
    s->dirty = true;
    return true;
}
