#include "renderer.h"
#include <stdlib.h>

// Default implementations that can be overridden by specific renderers

Renderer* renderer_create(RendererConfig *config) {
    // This is an abstract function - should be implemented by concrete renderers
    // Like cairo_renderer_create()
    (void)config; // Unused in abstract implementation
    return NULL;
}

// The rest of the functions are implemented in cairo_renderer.c
// This file exists mainly to satisfy the linker when building