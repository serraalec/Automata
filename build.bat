@echo off

goto comment
/* ========================================================================
$File: build.bat$
$Date: 5/22/2020$
$Revision: 1$
$Creator: Alec Serra $
    ======================================================================== */
:comment


taskkill /im win32_base.exe /F


set CommonCompilerFlags=-MT -WX -nologo -Gm- -GR- -EHa- -Od -Oi  -W4 -wd4100 -wd4101 -wd4201 -wd4189 -DDEBUG=1 -DHANDMADE_WIN32=1 -FC -Z7 -Fmwin32_handmade.map -DTODOS=0
set CommonLinkerFlags= -opt:ref user32.lib gdi32.lib winmm.lib /SUBSYSTEM:WINDOWS

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build

cl %CommonCompilerFlags% ..\code\win32_base.cpp /link %CommonLinkerFlags%

..\build\win32_base.exe

popd