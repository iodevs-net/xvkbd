#ifndef UI_EVENTS_H
#define UI_EVENTS_H

#include "x11_window.h"

void ui_event_callback(X11Window *window, WindowEvent *event, void *user_data);

#endif // UI_EVENTS_H
