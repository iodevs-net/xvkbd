#include "engine.h"
#include "layout.h"
#include "ui.h"
#include <stdio.h>

int main() {
    printf("Iniciando xvkbd_pro (Motor Cairo)...\n");
    
    EngineConfig config = {NULL, true, false};
    engine_init(&config);
    
    UIState ui;
    ui_init(&ui, layout_get_default());
    ui_loop(&ui);
    
    return 0;
}
