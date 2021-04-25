@echo off

:: build with Microsoft Visual Studio Community 2019
call "D:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" amd64

set PROJECT=cpumeter
set BUILD_DIR=release
rmdir /s /q %BUILD_DIR%
mkdir %BUILD_DIR%
pushd %BUILD_DIR%

cl /c /O1 ..\%PROJECT%.cpp /I "..\"
link %PROJECT%.obj advapi32.lib user32.lib pdh.lib gdi32.lib kernel32.lib /SUBSYSTEM:WINDOWS

:: Nullsoft Scriptable Install System
:: http://portableapps.com/apps/development/nsis_portable
if exist %PROJECT%.exe C:\MyPrograms\nsis\makensis.exe ..\%PROJECT%.nsi

popd

