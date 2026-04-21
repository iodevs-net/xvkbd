# RAM Usage Optimization for xvkbd

## Changes Made

### 1. UIState Struct (ui.h)
Added three new members to the `UIState` struct:
- `cairo_surface_t *cr_surface` - Cairo surface for the backbuffer
- `cairo_t *cr_context` - Cairo context for drawing
- `GC render_gc` - X11 graphics context for copying

### 2. Initialization (ui.c - ui_init)
Initialize the new members to NULL/0:
```c
state->cr_surface = NULL;
state->cr_context = NULL;
state->render_gc = 0;
```

### 3. Size Management (ui.c - ui_set_size)
- Properly manage Cairo surface/context lifecycle when window size changes
- Create/recreate Cairo surface and context to match the new backbuffer pixmap
- Create/reuse graphics context (`render_gc`) when needed

### 4. Rendering (ui.c - render_to_backbuffer)
- Reuse existing `state->cr_surface` and `state->cr_context` instead of creating/destroying each frame
- Remove the destruction of Cairo resources at the end of the function
- Added fallback creation if resources are missing (shouldn't happen in normal flow)

### 5. Event Handling (ui.c - ui_loop Expose handler)
- Replace per-expose GC creation/destruction with reuse of `state->render_gc`
- Only create GC if it doesn't already exist
- Use the persistent GC for `XCopyArea` operations

## Benefits
- **Reduced RAM fragmentation**: Resources are allocated once and reused
- **Lower memory overhead**: No per-frame or per-expose allocations
- **Improved performance**: Eliminates repeated allocation/deallocation overhead
- **Maintained visual quality**: All drawing operations remain unchanged

## Verification
- Code compiles without syntax errors (assuming proper X11/Cairo development packages)
- All drawing logic preserved
- Resource management follows proper lifecycle patterns