#ifndef X11_ENGINE_H
#define X11_ENGINE_H

#include "engine.h"

// X11-specific engine implementation
Engine* x11_engine_create(EngineConfig *config);

#endif // X11_ENGINE_H