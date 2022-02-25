#include "Config.h"
#include <math.h>

const char * Config::PluginName = "SpectrumBars\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0 © 2010-02-28 Tero Niemi (talamus@gmail.com) ";

int    Config::NumberOfBars         = Config::NumberOfBars_Default;
int    Config::SamplesPerBar        = Config::SamplesPerBar_Default;
real   Config::BarHeightPercent     = Config::BarHeightPercent_Default;
real   Config::BarSeparationPercent = Config::BarSeparationPercent_Default;
bool   Config::Logarithmic          = Config::Logarithmic_Default;
int    Config::Mode                 = Config::Mode_Default;
Colour Config::LowColour            = { 0x32, 0x67, 0x02, 0xff };  // Green
Colour Config::MediumColour         = { 0x00, 0xff, 0xff, 0xff };  // Yellow
Colour Config::HighColour           = { 0x00, 0x00, 0xff, 0xff };  // Red

bool Config::save(const char * filename)
{
    bool status = true;
    status &= writeInt(   "NumberOfBars",         NumberOfBars,         filename);
    status &= writeInt(   "SamplesPerBar",        SamplesPerBar,        filename);
    status &= writeFloat( "BarHeightPercent",     BarHeightPercent,     filename);
    status &= writeFloat( "BarSeparationPercent", BarSeparationPercent, filename);
    status &= writeBool(  "Logarithmic",          Logarithmic,          filename);
    status &= writeInt(   "Mode",                 Mode,                 filename);
    status &= writeColour("LowColour",            LowColour,            filename);
    status &= writeColour("MediumColour",         MediumColour,         filename);
    status &= writeColour("HighColour",           HighColour,           filename);
    return status;
}

bool Config::open(const char * filename)
{
    NumberOfBars     = readInt(    "NumberOfBars",     NumberOfBars,     NumberOfBars_Min,     NumberOfBars_Max,     filename);
    SamplesPerBar    = readInt(    "SamplesPerBar",    SamplesPerBar,    SamplesPerBar_Min,    SamplesPerBar_Max,    filename);
    BarHeightPercent = readFloat(  "BarHeightPercent", BarHeightPercent, BarHeightPercent_Min, BarHeightPercent_Max, filename);
    BarSeparationPercent = readFloat("BarSeparationPercent", BarSeparationPercent, BarSeparationPercent_Min, BarSeparationPercent_Max, filename);
    Logarithmic      = readBool(   "Logarithmic",      Logarithmic,                                                  filename);
    Mode             = readInt(    "Mode",             Mode,             Mode_Plain, Mode_Funky|Mode_Blurry,         filename);
    LowColour        = readColour( "LowColour",        LowColour,                                                    filename);
    MediumColour     = readColour( "MediumColour",     MediumColour,                                                 filename);
    HighColour       = readColour( "HighColour",       HighColour,                                                   filename);
    return true;
}

//
// General tools used here
//
namespace ConfigTools {

// How many chars are required for the integer?
int digits(int i, int sign = 0) {
    if (i == 0) return sign + 1;
    if (i < 0) {
        sign = 1;
        i *= -1;
    }
    return sign + 1 + int(log10(i));
}

// Buffer for read and write functions (No thread safety whatsoever!)
char IOBuffer[512];

} // namespace ConfigTools

//
// Reading
//

int Config::readInt(const char * name, int current, int min, int max, const char * filename)
{
    char * str = readString(name, filename);
    int i;
    if (sscanf(str, "%u", &i) == 1) {
        i = i > max ? max : i < min ? min : i;
        return i;
    } else {
        return current;
    }
}

real Config::readFloat(const char * name, real current, real min, real max, const char * filename)
{
    char * str = readString(name, filename);
    real f;
    if (sscanf(str, "%f", &f) == 1) {
        f = f > max ? max : f < min ? min : f;
        return f;
    } else {
        return current;
    }
}

bool Config::readBool(const char * name, bool current, const char * filename)
{
    char * str = readString(name, filename);
    int i;
    if (str[0] == 't' || str[0] == 'T' || str[0] == '1') {
        return true;
    } else if (str[0] == 'f' || str[0] == 'F' || str[0] == '0') {
        return false;
    } else {
        return current;
    }
}

Colour Config::readColour(const char * name, const Colour & current, const char * filename)
{
    ASSERT(sizeof(unsigned long) >= sizeof(Colour));
    char * str = readString(name, filename);
    unsigned long i;
    if (sscanf(str, "%x", &i) == 1) {
        Colour c;
        c.blue  = i & 0xff; i >>= 8;
        c.green = i & 0xff; i >>= 8;
        c.red   = i & 0xff; i >>= 8;
        c.alpha = i & 0xff;
        return c;
    } else {
        return current;
    }
}

char * Config::readString(const char * name, const char * filename)
{
    GetPrivateProfileString(Config::PluginName, name, "", ConfigTools::IOBuffer, sizeof(ConfigTools::IOBuffer), filename);
    ConfigTools::IOBuffer[sizeof(ConfigTools::IOBuffer)-1] = '\0';
    return ConfigTools::IOBuffer;
}

//
// Writing
//

bool Config::writeInt(const char * name, int current, const char * filename)
{
    sprintf(ConfigTools::IOBuffer, "%d", current);
    ConfigTools::IOBuffer[sizeof(ConfigTools::IOBuffer)-1] = '\0';
    return writeString(name, ConfigTools::IOBuffer, filename);
}

bool Config::writeFloat(const char * name, real current, const char * filename)
{
    if (sprintf(ConfigTools::IOBuffer, "%g", current) == ConfigTools::digits(int(current))) {
        sprintf(ConfigTools::IOBuffer, "%.1f", current);
    }
    ConfigTools::IOBuffer[sizeof(ConfigTools::IOBuffer)-1] = '\0';
    return writeString(name, ConfigTools::IOBuffer, filename);
}

bool Config::writeBool(const char * name, bool current, const char * filename)
{
    if (current) {
        return writeString(name, "true", filename);
    } else {
        return writeString(name, "false", filename);
    }
}

bool Config::writeColour(const char * name, const Colour & current, const char * filename)
{
    sprintf(ConfigTools::IOBuffer, "0x%02X%02X%02X%02X", current.alpha, current.red, current.green, current.blue);
    ConfigTools::IOBuffer[sizeof(ConfigTools::IOBuffer)-1] = '\0';
    return writeString(name, ConfigTools::IOBuffer, filename);
}

bool Config::writeString(const char * name, const char * current, const char * filename)
{
    return WritePrivateProfileString(Config::PluginName, name, current, filename);
}

// eof
