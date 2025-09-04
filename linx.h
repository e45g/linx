#ifndef _LINX_H
#define _LINX_H

#include <X11/Xlib.h>

typedef struct {
    int screen;
    GC gc;
    Display *display;
    Window window;
} linx_t;

linx_t *linx_init(int width, int height, const char *title);

void linx_clear(linx_t *lx);
void linx_close(linx_t *lx);

int linx_setcolor_rgb(linx_t *lx, unsigned short red, unsigned short green, unsigned short blue);
void linx_mainloop(linx_t *lx, void (*draw)(linx_t*));

void linx_drawline(linx_t *lx, int x1, int y1, int x2, int y2);
void linx_drawcircle(linx_t *lx, int x, int y, int radius);

#endif
