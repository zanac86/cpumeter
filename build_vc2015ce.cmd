@echo off

set BUILD_DIR=release

erase /F /S /Q *.bak *.orig
rmdir /S /Q %BUILD_DIR%
mkdir %BUILD_DIR%

call "D:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
rc cpumeter.rc
cl cpumeter.cpp cpumeter.res /MT /link advapi32.lib user32.lib pdh.lib gdi32.lib

copy cpumeter.exe release\
erase cpumeter.exe cpumeter.obj cpumeter.res

C:\MyPrograms\NSISPortable\App\NSIS\makensis.exe cpumeter.nsi
