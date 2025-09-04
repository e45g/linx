#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdlib.h>
#include <unistd.h>

#include "linx.h"

linx_t *linx_init(int width, int height, const char *title) {
    linx_t *lx = malloc(sizeof(linx_t));
    lx->display = XOpenDisplay(NULL);
    lx->screen = DefaultScreen(lx->display);
    lx->window = XCreateSimpleWindow(
        lx->display,
        RootWindow(lx->display, lx->screen),
        10,
        10,
        width,
        height,
        1,
        WhitePixel(lx->display, lx->screen),
        BlackPixel(lx->display, lx->screen)
    );

    XStoreName(lx->display, lx->window, title);
    XSelectInput(lx->display, lx->window, ExposureMask | KeyPressMask);
    XMapWindow(lx->display, lx->window);

    lx->gc = XCreateGC(lx->display, lx->window, 0, NULL);
    XSetForeground(lx->display, lx->gc, WhitePixel(lx->display, lx->screen));

    return lx;
}

void linx_mainloop(linx_t *lx, void (*draw)(linx_t *)) {
    int running = 1;

    while(running) {
        // while(XPending(lx->display)) {
            XEvent e;
            XNextEvent(lx->display, &e);

            if(e.type == KeyPress) {
                running = 1; // TODO: handle key press
            }
        // }
        linx_clear(lx);

        draw(lx);
        XFlush(lx->display);
        usleep(16000); // 60fps
    }
}

void linx_clear(linx_t *lx) {
    XClearWindow(lx->display, lx->window);
}

void linx_close(linx_t *lx) {
    XCloseDisplay(lx->display);
    free(lx);
}

void linx_drawline(linx_t *lx, int x1, int y1, int x2, int y2) {
    XDrawLine(lx->display, lx->window, lx->gc, x1, y1, x2, y2);
}

void linx_drawcircle(linx_t *lx, int x, int y, int radius) {
    XDrawArc(lx->display, lx->window, lx->gc, x, y, radius, radius, 0, 360*64);
}

int linx_setcolor_rgb(linx_t *lx, unsigned short red, unsigned short green, unsigned short blue) {
    XColor color;
    Colormap colormap = DefaultColormap(lx->display, lx->screen);

    color.red = red << 8;
    color.green = green << 8;
    color.blue = blue << 8;
    color.flags = DoRed | DoGreen | DoBlue;

    if (XAllocColor(lx->display, colormap, &color) == 0) {
        return -1;
    }

    XSetForeground(lx->display, lx->gc, color.pixel);

    return 0;
}
