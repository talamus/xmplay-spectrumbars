#ifndef GRAPHIC_H
#define GRAPHIC_H

//
//  A simple graphics library
//

#include "real.h"
#include "Debug.h"
namespace Gfx {

//
//  Colour information
//
class Colour {
  public:
    byte blue;  // Colour elements
    byte green;
    byte red;
    byte alpha;
    inline void merge(const Colour & c, const byte alpha);  // Merge this colour with another
};

//
//  How to fill an area?
//
class Fill
{
  public:
    virtual const Colour getColour(const int x, const int y) const = 0;
};

//
//  Painter class
//
class Painter
{
  protected:
    Colour * video_;    // Actual pixels
    int width_;         // Video memory width
    int height_;        // Video memory height
    int pitch_;         // Video memory pitch

  public:
    Painter(unsigned long * video, int width, int height, int pitch);  // Init painter

    void clearScreen(byte value = 0x00);  // Clear the whole screen with value

    inline Colour * drawPixel(const int x, const int y, const Colour c, const byte alpha = 0xff);
                        // Draw pixel to (x,y) with alpha, return next pixel on the row

    inline Colour * drawPixel(Colour * video, const Colour c, const byte alpha = 0xff);
                        // Draw pixel to video position with alpha, return next pixel on the row

    void drawRect(real fx, real fy, real fwidth, real fheight, const Fill & f, byte alpha = 0xff);
                        // Fill a translucent and anti-aliased box
    #ifndef NDEBUG
    void printVideo(void);
    #endif // NDEBUG
};

// Inline function code

inline void Colour::merge(const Colour & fg, const byte fg_alpha)
{
    if (!fg_alpha) return;
    const int bg_alpha = fg_alpha ^ 0xff;
    if (!bg_alpha) {
        *this = fg;
        return;
    }
    blue  = ((int) blue  * bg_alpha >> 8) + ((int) fg.blue  * fg_alpha >> 8);
    green = ((int) green * bg_alpha >> 8) + ((int) fg.green * fg_alpha >> 8);
    red   = ((int) red   * bg_alpha >> 8) + ((int) fg.red   * fg_alpha >> 8);
    alpha = ((int) alpha * bg_alpha >> 8) + ((int) fg.alpha * fg_alpha >> 8);
}

inline Colour * Painter::drawPixel(const int x, const int y, const Colour c, const byte alpha)
{
    ASSERT(x >= 0);
    ASSERT(y >= 0);
    ASSERT(x < width_);
    ASSERT(y < height_);
    return drawPixel(video_ + x + y*pitch_, c, alpha);
}

inline Colour * Painter::drawPixel(Colour * v, const Colour c, const byte alpha)
{
    ASSERT(v >= video_);
    ASSERT(v < video_ + width_ * height_);
    if (alpha) v->merge(c, (int) alpha * c.alpha >> 8);
    return ++v;
}

} // namespace Gfx
#endif // GRAPHIC_H
