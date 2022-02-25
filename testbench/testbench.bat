@echo off
del testbench.exe 2> nul

g++ -o testbench.exe testbench.cpp Graphic.cpp Debug.cpp

rem Release version:
rem
rem    g++ -O3 -DNDEBUG -o testbench.exe testbench.cpp Graphic.cpp Debug.cpp
rem
testbench.exe
