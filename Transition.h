#ifndef TRANSITION_H
#define TRANSITION_H

//
//  Numeric transitions for both integer and real types
//
//  Real transitions are from [0.0 .. 1.0] to [0.0 .. 1.0]
//  Integer transitions are from [0..255] to [0..255]
//

#include "real.h"
namespace Transition {

//
//  Real transitions
//

inline real linear(real x)              // Linear (just to demonstrate the basic idea...)
{
    return x;
}

inline real smooth(real x)              // Wide middle section
{
    return x*(2.0*x*x - 3.0*x + 2.0);
}

inline real smoother(real x)            // Even wider middle section
{
    return x*(4.0*x*x - 6.0*x + 3.0);
}

inline real bottom(real x)              // Bottom weighted
{
    return x*(x*x + 1.0)/2.0;
}

inline real top(real x)                 // Top weighted
{
    return x*(x*x - 3.0*x + 4.0)/2.0;
}

inline real onOff(real x)               // Parable [0.0 .. 1.0] -> [0.0 .. 1.0 .. 0.0]
{
    return 4.0*x - 4.0*x*x;
}

inline real invert(real transition)     // Invert transition [0.0 .. 1.0] -> [1.0 .. 0.0]
{
    return 1.0 - transition;
}

//
//  Integer transitions
//

inline int linearInt(int x)             // Linear (just to demonstrate the basic idea...)
{
    return x;
}

inline int smoothInt(int x)             // Wide middle section
{
    return x*(x*x - (3*x << 7) + (2 << 15)) >> 15;
}

inline int smootherInt(int x)           // Even wider middle section
{
    return (x*(x*x - (6*x << 6) + (3 << 14)) >> 14) + 1;
}

inline int bottomInt(int x)             // Bottom weighted
{
    return x*(x*x | (1 << 16)) >> 17;
}

inline int topInt(int x)                // Top weighted
{
    return x*(x*x - (3*x << 8) + (1 << 18)) >> 17;
}

inline int onOffInt(int x)              // Parable [0..255] -> [0..255..0]
{
    return x - (x*x / 255) << 2;
}

inline int invertInt(int transition)    // Invert transition [0..255] -> [255..0]
{
    return transition ^ 255;
}

} // namespace Transition

#ifdef TESTBENCH
#include <stdio.h>
int main(void) {
    printf("linear   (diff) ");
    printf("smooth   (diff) ");
    printf("smoother (diff) ");
    printf("bottom   (diff) ");
    printf("top      (diff) ");
    printf("onOff    (diff) ");
    printf("invert   (diff)\n");
    for (int x = 0; x < 256; ++x) {
        int f, i;
        f = int(255.0 * Transition::linear(real(x) / 255.0));
        i =             Transition::linearInt(x);
        printf(" %3d %3d  (%+d)  ", f, i, f-i);
        f = int(255.0 * Transition::smooth(real(x) / 255.0));
        i =             Transition::smoothInt(x);
        printf(" %3d %3d  (%+d)  ", f, i, f-i);
        f = int(255.0 * Transition::smoother(real(x) / 255.0));
        i =             Transition::smootherInt(x);
        printf(" %3d %3d  (%+d)  ", f, i, f-i);
        f = int(255.0 * Transition::bottom(real(x) / 255.0));
        i =             Transition::bottomInt(x);
        printf(" %3d %3d  (%+d)  ", f, i, f-i);
        f = int(255.0 * Transition::top(real(x) / 255.0));
        i =             Transition::topInt(x);
        printf(" %3d %3d  (%+d)  ", f, i, f-i);
        f = int(255.0 * Transition::onOff(real(x) / 255.0));
        i =             Transition::onOffInt(x);
        printf(" %3d %3d  (%+d)  ", f, i, f-i);
        f = int(255.0 * Transition::invert(real(x) / 255.0));
        i =             Transition::invertInt(x);
        printf(" %3d %3d  (%+d)\n", f, i, f-i);
    }
    return 0;
}
#endif // TESTBENCH

#endif // TRANSITION_H
