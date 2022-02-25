#include "Graphic.h"
#include <string.h>
namespace Gfx {

Painter::Painter(unsigned long * video, int width, int height, int pitch)
    : video_((Colour *) video), width_(width), height_(height), pitch_(pitch) { }

void Painter::clearScreen(byte value)
{
    memset(video_, value, sizeof(Colour) * width_ * height_);
}

void Painter::drawRect(real fx, real fy, real fwidth, real fheight, const Fill & f, byte alpha)
{
    // Handle negative dimensions
    if (fwidth <= 0.0) {
        if (fwidth == 0.0) return;
        fx += fwidth;
        fwidth *= -1.0;
    }
    if (fheight <= 0.0) {
        if (fheight == 0.0) return;
        fy += fheight;
        fheight *= -1.0;
    }

    // Handle negative top left coordinates
    if (fx < 0.0) {
        if (fx + fwidth <= 0.0) return;
        fwidth += fx;
        fx = 0.0;
    }
    if (fy < 0.0) {
        if (fy + fheight <= 0.0) return;
        fheight += fy;
        fy = 0.0;
    }

    int x = int(fx);  // Top left pixel
    int y = int(fy);

    // Out of screen?
    if (x >= width_) return;
    if (y >= height_) return;

    const int x2 = int(fx + fwidth);  // Bottom right pixel
    const int y2 = int(fy + fheight);

    Colour * v;  // Ponter to video memory

    // Pre-calculated alpha values
    const byte alpha_top         = byte(alpha * (y + 1.0 - fy));
    const byte alpha_left        = byte(alpha * (x + 1.0 - fx));
    const byte alpha_right       = byte(alpha * (fx + fwidth - x2));
    const byte alpha_bottom      = byte(alpha * (fy + fheight - y2));
    const byte alpha_topleft     = byte(alpha * (y + 1.0 - fy) * (x + 1.0 - fx));
    const byte alpha_topright    = byte(alpha * (y + 1.0 - fy) * (fx + fwidth - x2));
    const byte alpha_bottomleft  = byte(alpha * (fy + fheight - y2) * (x + 1.0 - fx));
    const byte alpha_bottomright = byte(alpha * (fy + fheight - y2) * (fx + fwidth - x2));

    // Draw top row
    v = drawPixel(x, y, f.getColour(x, y), alpha_topleft);
    for (++x; x < x2 && x < width_; ++x) {
        v = drawPixel(v, f.getColour(x, y), alpha_top);
    }
    if (x == x2 && x < width_) {
        v = drawPixel(v, f.getColour(x, y), alpha_topright);
    }

    // Draw middle area
    for (++y; y < y2 && y < height_; ++y) {
        x = int(fx);
        v = drawPixel(x, y, f.getColour(x, y), alpha_left);
        for (++x; x < x2 && x < width_; ++x) {
            v = drawPixel(v, f.getColour(x, y), alpha);
        }
        if (x == x2 && x < width_) {
            v = drawPixel(v, f.getColour(x, y), alpha_right);
        }
    }

    // Draw bottom row
    if (y == y2 && y < height_) {
        x = int(fx);
        v = drawPixel(x, y, f.getColour(x, y), alpha_bottomleft);
        for (++x; x < x2 && x < width_; ++x) {
            v = drawPixel(v, f.getColour(x, y), alpha_bottom);
        }
        if (x == x2 && x < width_) {
            v = drawPixel(v, f.getColour(x, y), alpha_bottomright);
        }
    }
}

#ifndef NDEBUG
void Painter::printVideo(void)
{
    Colour * v = video_;
    for (int x = 0; x < width_; x++) {
        PRINTF(" %8d", x);
    }
    PRINTF("\n");
    for (int y = height_; y--;) {
        PRINTF("%3d", height_ - y - 1);
        for (int x = width_; x--;) {
            PRINTF(" %02X%02X%02X%02X", v->red, v->green, v->blue, v->alpha);
            ++v;
        }
        PRINTF("\n");
    }
}
#endif // NDEBUG

} // namespace Gfx
