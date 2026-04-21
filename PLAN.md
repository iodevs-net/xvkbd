# Optimization Plan for xvkbd RAM usage

## Problem
The current implementation creates new Cairo surfaces and contexts on every render cycle, causing high RAM usage despite proper cleanup. Additionally, graphics contexts (GC) are created and destroyed per Expose event.

## Solution
Reuse Cairo surfaces and contexts across frames, recreating only when size changes. Reuse X11 GC instead of creating per event.

## Changes

### ui.h
Add to UIState struct:
- `cairo_surface_t *cr_surface;`
- `cairo_t *cr_context;`
- `GC render_gc;`

### ui.c
1. In `ui_init`: initialize the above members to NULL/0.
2. In `ui_set_size`: 
   - If cr_surface exists and size changed, destroy old surface/context.
   - Create new cairo surface matching pixmap dimensions.
   - Create cairo context from surface.
   - Create/reuse GC (if graphics context changed due to depth change, recreate).
3. In `render_to_backbuffer`:
   - Use existing cr_surface and cr_context (ensure they match backbuffer size).
   - Clear surface with `cairo_set_operator` and `cairo_paint`.
   - Proceed with drawing as before.
   - Do NOT destroy surface/context at end.
4. In `ui_loop` Expose handler:
   - Use existing render_gc (create if NULL) instead of `XCreateGC`/`XFreeGC` each time.
   - After copying area, if render_gc was just created, keep it for reuse.
5. Add cleanup function `ui_cleanup(UIState *)` to free Cairo resources and GC, called before exit.

## Expected Impact
- Eliminates per-frame allocation of Cairo surfaces/contexts.
- Eliminates per-expose GC allocation.
- Reduces RAM fragmentation and overall footprint.
- Maintains visual quality and versatility (theming, scaling, menu).