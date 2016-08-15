call "D:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
rc cpumeter.rc
cl cpumeter.cpp cpumeter.res /MT /link advapi32.lib user32.lib pdh.lib gdi32.lib