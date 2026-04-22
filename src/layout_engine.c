/*
 * 0-Board Virtual Keyboard
 * Copyright (c) 2026 Leonardo Vergara <leonardovergaramarin@gmail.com>
 * Licensed under the MIT License.
 */
#include "layout_engine.h"
#include <stdlib.h>

int layout_engine_calculate(const Layout *layout, int win_width, int win_height, 
                           int menu_offset, Rectangle *out_bounds) {
    if (!layout || !out_bounds || layout->num_keys == 0) return 0;
    
    int kb_width = win_width;
    int kb_height = win_height - menu_offset;
    int pad = kb_width * KEY_PADDING_RATIO;
    int gap = kb_width * KEY_GAP_RATIO;
    
    // 1. Count rows
    int num_rows = 0;
    for (int i = 0; i < layout->num_keys; i++) {
        if (layout->keys[i].new_row) num_rows++;
    }
    if (num_rows == 0) num_rows = 1;
    
    int row_h = (kb_height - 2*pad) / num_rows;
    
    // 2. Calculate keys row by row
    int current_row = -1;
    int row_start = 0;
    
    for (int i = 0; i < layout->num_keys; i++) {
        if (layout->keys[i].new_row) {
            if (current_row >= 0) {
                // Process previous row
                double row_weight = 0;
                for (int j = row_start; j < i; j++)
                    row_weight += layout->keys[j].width_weight;
                
                double unit_w = (kb_width - 2*pad - (i - row_start - 1) * gap) / row_weight;
                int cx = pad;
                for (int j = row_start; j < i; j++) {
                    out_bounds[j].x = cx;
                    out_bounds[j].y = menu_offset + pad + current_row * row_h;
                    out_bounds[j].width = layout->keys[j].width_weight * unit_w;
                    out_bounds[j].height = row_h - gap;
                    cx += out_bounds[j].width + gap;
                }
            }
            current_row++;
            row_start = i;
        }
    }
    
    // Process last row
    if (current_row >= 0 && row_start < layout->num_keys) {
        int count = layout->num_keys - row_start;
        double row_weight = 0;
        for (int j = row_start; j < layout->num_keys; j++)
            row_weight += layout->keys[j].width_weight;
        
        double unit_w = (kb_width - 2*pad - (count - 1) * gap) / row_weight;
        int cx = pad;
        for (int j = row_start; j < layout->num_keys; j++) {
            out_bounds[j].x = cx;
            out_bounds[j].y = menu_offset + pad + current_row * row_h;
            out_bounds[j].width = layout->keys[j].width_weight * unit_w;
            out_bounds[j].height = row_h - gap;
            cx += out_bounds[j].width + gap;
        }
    }
    
    return layout->num_keys;
}
