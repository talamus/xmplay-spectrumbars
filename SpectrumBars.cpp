#include "SpectrumBars.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>

void Initialise(void) { }

bool Render(unsigned long * video, const int width, const int height, const int pitch, const Vis::Data * newData)
{
    static bool firstRender = true;        // Is this the first time we render anything?
    static Vis::Data data = {-1, {}, {}};  // When paused (MilliSec is zero) we show this data

    static TriGradient Gradient(Config::HighColour, Config::MediumColour, Config::LowColour);  // Three colour gradient

    // Update data if new is available
    if (newData->MilliSec) {
        memcpy(&data, newData, sizeof(Vis::Data));
    }

    // Do not do anything if we got nothing to show
    if (data.MilliSec <= 0) return false;

    // Initialise graphic
    Painter p(video, width, height, pitch);

    // Clear screen
    if (firstRender || !(Config::Mode & Config::Mode_Blurry)) {
        p.clearScreen();
    }

    // Set scale
    const real bar_width = 100.0;  // percent
    const real x_scale = real(width) / (Config::NumberOfBars * bar_width + Config::BarSeparationPercent);
    const real y_scale = real(height);
    Gradient.setScale(height / 4, height - 1);

    // Spectrum range we are showing
    int min = (Config::Logarithmic || Config::SamplesPerBar == 1) ? (0) : (1);
    int max = (Config::SamplesPerBar * Config::NumberOfBars > Vis::Spectrum_Length)
            ? (Vis::Spectrum_Length) : (Config::SamplesPerBar * Config::NumberOfBars);

    // Create bars
    real bars[Config::NumberOfBars];
    memset(&bars, 0x00, sizeof(bars));
    for (int i = min; i < max; ++i) {

        // Two channel sum
        real value = data.Spectrum[Vis::Left][i] + data.Spectrum[Vis::Right][i];

        if (Config::Logarithmic) {

            // If not paused add jitter to hide the poor resolution
            if (newData->MilliSec && value > 0.0) {
                value += 2.0 * real(rand()) / real(RAND_MAX) - 1.0;
            }
            if (value < 0.0) value = 0.0;

            // Logarithmic correction
            value *= log(1.1 + i) / 4.0;
        }

        // Downscaling
        value *= 2.5 * Config::BarHeightPercent / 100.0 / real(Vis::Spectrum_Max);

        bars[i / Config::SamplesPerBar] = value;
    }

    // Draw bars
    for (int i = Config::NumberOfBars; i--;) {
        p.drawRect(
            x_scale * (i * bar_width + Config::BarSeparationPercent),

            (Config::Mode & Config::Mode_Funky)
             ? y_scale * (1.0 - 2.0 * 0.666 * bars[i]) - (Config::BarSeparationPercent * x_scale)
             : y_scale * (1.0 - bars[i])               - (Config::BarSeparationPercent * x_scale),

            x_scale * (bar_width - Config::BarSeparationPercent),

            (Config::Mode & Config::Mode_Funky)
             ? y_scale * 0.666 * bars[i]
             : y_scale * bars[i],

            Gradient,

            (Config::Mode & Config::Mode_Blurry) ? (0x50) : (0xff)
        );
    }

    // All done! Exit
    firstRender = false;
    return true;
}

bool OpenSettings(const char * filename)
{
    return Config::open(filename);
}

bool SaveSettings(const char * filename)
{
    return Config::save(filename);
}

bool Deinit()
{
    return true;
}

bool Clicked(const int x, const int y, const int buttons)
{
    ++Config::Mode %= 4;
    return true;
}

bool ReceiveQueryInterface(const Vis::QueryInterface * i)
{
    return true;
}

// eof
