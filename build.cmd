@echo off
set path=D:\Qt\qt-5.5.0-x64-mingw510r0-seh-rev0\mingw64\bin
set BUILD_DIR=release

erase /F /S /Q *.exe *.bak *.orig
rmdir /S /Q %BUILD_DIR%
mkdir %BUILD_DIR%
mingw32-make

C:\MyPrograms\NSISPortable\App\NSIS\makensis.exe cpumeter.nsi
