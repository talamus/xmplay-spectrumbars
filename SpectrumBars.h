#ifndef SPECTRUMBARS_H
#define SPECTRUMBARS_H

//
//  SpectrumBars - A basic spectrum style plug-in for XMPlay
//
//   - Simple but polished
//   - Four modes, click with middle button to change
//   - Fully configurable via the vis.ini file
//   - Linear or logarithmic scale
//   - Source code included
//
//  (c) 2010-02-28 Tero Niemi (talamus@gmail.com)
//

#include "Vis.h"            // XMPlay/Sonique visualisation interface
#include "Config.h"         // Configuration variables
#include "Graphic.h"        // A simple graphics library
#include "TriGradient.h"    // Three colour vertical gradient
using namespace Gfx;

void Initialise(void);
bool Render(unsigned long * video, int width, int height, int pitch, const Vis::Data * data);
bool SaveSettings(const char * filename);
bool OpenSettings(const char * filename);
bool Deinit(void);
bool Clicked(int x, int y, int buttons);
bool ReceiveQueryInterface(const Vis::QueryInterface * i);

Vis::Info Plugin = {
    2,
    Config::PluginName,
    Vis::WaveformRequest | Vis::SpectrumRequest | Vis::PostProcessing,
    &Initialise,
    &Render,
    &SaveSettings,
    &OpenSettings,
    &Deinit,
    &Clicked,
    &ReceiveQueryInterface
};

#endif // SPECTRUMBARS_H
