@echo off
echo.
echo 	Make Release...
echo.

del SpectrumBars.svp >nul 2>&1
del *.o >nul 2>&1

g++ -DNDEBUG -O3 -c SpectrumBars.cpp Graphic.cpp Debug.cpp TriGradient.cpp Config.cpp
g++ -shared -o SpectrumBars.svp SpectrumBars.o Graphic.o Debug.o TriGradient.o Config.o

echo.
if [%ERRORLEVEL%]==[0] (echo 	All Ok!) else (echo 	Oh noes...)
echo.
rem	copy SpectrumBars.svp C:\Music\XMPlay\Vis
