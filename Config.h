#ifndef CONFIG_H
#define CONFIG_H

//
//  Configuration
//

#include "Graphic.h"
#include <stdio.h>
#include <windows.h>
using namespace Gfx;

class Config
{
  public:
    static const char * PluginName;                // Our identity

    static int        NumberOfBars;                // Number of bars on the screen
    static const int  NumberOfBars_Min             = 1;
    static const int  NumberOfBars_Default         = 25;
    static const int  NumberOfBars_Max             = 255;

    static int        SamplesPerBar;               // Number of spectrum samples per bar
    static const int  SamplesPerBar_Min            = 1;
    static const int  SamplesPerBar_Default        = 2;
    static const int  SamplesPerBar_Max            = 255;

    static real       BarHeightPercent;            // Height of bars (in percent)
    static const real BarHeightPercent_Min         = 0.1;
    static const real BarHeightPercent_Default     = 100.0;
    static const real BarHeightPercent_Max         = 10000.0;

    static real       BarSeparationPercent;        // Width separation of bars (in percent)
    static const real BarSeparationPercent_Min     = -100.0;
    static const real BarSeparationPercent_Default = 10.0;
    static const real BarSeparationPercent_Max     = 99.99;

    static bool       Logarithmic;                 // Use logarithmic scale?
    static const bool Logarithmic_Default          = true;

    static int        Mode;                        // Display mode
    static const int  Mode_Plain                   = 0x00;
    static const int  Mode_Blurry                  = 0x01;
    static const int  Mode_Funky                   = 0x02;
    static const int  Mode_Default                 = Mode_Funky|Mode_Blurry;

    static Colour     LowColour;                   // See Config.cpp for default colour values
    static Colour     MediumColour;
    static Colour     HighColour;

    static bool open(const char * filename);       // Load configuration from a file
    static bool save(const char * filename);       // Save configuration to a file

  private:
    // Read and parse values
    static int    readInt(   const char * name, int current, int min, int max,    const char * filename);
    static real   readFloat( const char * name, real current, real min, real max, const char * filename);
    static bool   readBool(  const char * name, bool current,                     const char * filename);
    static Colour readColour(const char * name, const Colour & current,           const char * filename);
    static char * readString(const char * name,                                   const char * filename);

    // Write values
    static bool writeInt(    const char * name, int current,                      const char * filename);
    static bool writeFloat(  const char * name, real current,                     const char * filename);
    static bool writeBool(   const char * name, bool current,                     const char * filename);
    static bool writeColour( const char * name, const Colour & current,           const char * filename);
    static bool writeString( const char * name, const char * current,             const char * filename);
};

#endif // CONFIG_H
