#ifndef VIS_H
#define VIS_H

//
//  XMPlay/Sonique visualisation interface
//

namespace Vis {

enum {
    Channels = 2, Left = 0, Right = 1, Volume = 255,
    Waveform_Length = 512, Waveform_Max = 127, Waveform_Min = -128,
    Spectrum_Length = 255, Spectrum_Max = 255, Spectrum_Min = 0
};

enum Flags { WaveformRequest = 0x01, SpectrumRequest = 0x02, PostProcessing = 0x04 };

//
//  Music data to be visualised
//
struct Data
{
    long           MilliSec;
    char           Waveform[Channels][Waveform_Length];
    unsigned char  Spectrum[Channels][Spectrum_Length + 1];  // Last one is volume: Spectrum[Left][Volume]
};

//
//  Query interface for additional information
//
//  You can pass any of these strings to query as expression and be returned a value:
//
//      QueryInt:
//          currentsonglength  currentsongposition  scheme_visual_left  scheme_visual_right  scheme_visual_blend
//
//      QueryString:
//          currentskinname  currentsongfilename  currentsongtitle  currentsongauthor  currengsongdisplaystring
//
//  Make sure to free it later if it's a string!
//
class QueryInterface
{
  public:
    virtual bool    QueryInt(const char * expression, int * value);
    virtual char *  QueryString(const char * expression);
    virtual void    FreeString(char * string);
};

//
//  Visualisation plugin information
//
struct Info
{
    const unsigned long  Version;     // == 2
    const char *         PluginName;
    const unsigned long  Flags;
    void  (*Initialise)(void);
    bool  (*Render)(unsigned long * video, const int width, const int height, const int pitch, const Data * data);
    bool  (*SaveSettings)(const char * filename);
    bool  (*OpenSettings)(const char * filename);
    bool  (*Deinit)(void);
    bool  (*Clicked)(const int x, const int y, const int buttons);
    bool  (*ReceiveQueryInterface)(const QueryInterface * i);
};

} // namespace Vis

//
//  Plugin to be exported
//
extern Vis::Info Plugin;

//
//  DLL export
//
extern "C" __declspec(dllexport) const Vis::Info * QueryModule(void)
{
    return &Plugin;
}

#endif // VIS_H
