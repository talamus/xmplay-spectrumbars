#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//
//  Test bench for graphics library
//

#include "Graphic.h"
using namespace Gfx;

class Solid : public Fill
{
  public:
    inline const Colour getColour(const int x, const int y) const {
        Colour c = { 0xff, 0xff, 0xff, 0xff };
        return c;
    }
};

float frand() {
    return ((float) rand()) / ((float) RAND_MAX);
}

int main(void)
{
    srand(time(0));

    long rectangles = 10000000;
    int width = 10;
    int height = 12;
    float fw = width;
    float fh = height;
    Colour video[width * height];
    Painter p((long unsigned int *) &video, width, height, width);
    Solid fill;
    p.clearScreen();

    printf("\nDrawing rectangles...\n\n");

    clock_t start = clock();

    for (long i = rectangles; i--;) {
        float x1 = frand() * fw * 2.0 - fw / 2.0;
        float y1 = frand() * fh * 2.0 - fh / 2.0;
        float x2 = frand() * fw * 2.0 - fw / 2.0;
        float y2 = frand() * fh * 2.0 - fh / 2.0;
        p.drawRect(x1,y1,x2,y2,fill);
    }

    clock_t end = clock();

    #ifndef NDEBUG
    p.printVideo();
    #endif
    printf("\n%d rectangles in %f seconds\n", rectangles, double(end - start) / double(CLOCKS_PER_SEC));

    return 0;
}

// eof
