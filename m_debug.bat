@echo off
echo.
echo 	Make Debug...
rem
rem	Add -DTN_DEBUG_LOG to the compile if you need debug logging.
rem

del SpectrumBars.svp >nul 2>&1
del *.o >nul 2>&1

g++ -c SpectrumBars.cpp Graphic.cpp Debug.cpp TriGradient.cpp Config.cpp
g++ -shared -o SpectrumBars.svp SpectrumBars.o Graphic.o Debug.o TriGradient.o Config.o

echo.
if [%ERRORLEVEL%]==[0] (echo 	All Ok!) else (echo 	Oh noes...)
echo.
rem	copy SpectrumBars.svp C:\Music\XMPlay\Vis
