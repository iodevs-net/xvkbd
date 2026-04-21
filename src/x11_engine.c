#include "x11_engine.h"
#include "engine.h"

// For now, x11_engine is just a wrapper around the generic engine
// This allows for future platform-specific optimizations

Engine* x11_engine_create(EngineConfig *config) {
    return engine_create(config);
}