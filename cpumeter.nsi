SetCompressor /SOLID lzma

Name "CpuMeter"
OutFile "release\Setup_CpuMeter_x64.exe"
InstallDir "$PROGRAMFILES64\CpuMeter"
ShowInstDetails hide
ShowUninstDetails show

;AutoCloseWindow true

VIAddVersionKey  "ProductName" "CpuMeter"
VIAddVersionKey  "Comments" "Monitor CPU usage by perfomance counter. You can change settings in registry: HKEY_CURRENT_USER\Software\vb\CpuMeter"
VIAddVersionKey  "CompanyName" "VB Lab"
VIAddVersionKey  "LegalCopyright" "VB Lab"
VIAddVersionKey  "FileDescription" "CpuMeter installer"
VIAddVersionKey  "FileVersion" "1.2.0.0"
VIProductVersion "1.2.0.0"

;RequestExecutionLevel user
RequestExecutionLevel highest
;RequestExecutionLevel admin

;icon "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"

Page directory
Page components
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;-------------------------------------------------------------------------------
Section "-Main"
  ExecWait '"taskkill.exe" /F /IM CpuMeter.exe'
  SetOutPath '$INSTDIR'
  File '.\release\cpumeter.exe'
  WriteUninstaller "uninstall.exe"
  DeleteRegKey HKCU "Software\vb\CpuMeter"

  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CpuMeter" "DisplayName"     "CpuMeter (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CpuMeter" "UninstallString" '"$INSTDIR\uninstall.exe"'

;  Messagebox MB_OK|MB_ICONINFORMATION \
;    "Click shift + right button on CpuMeter window to open REGEDIT"

SectionEnd

Section "Shortcuts in Start Menu"
  CreateDirectory "$SMPROGRAMS\CpuMeter"
  CreateShortCut  "$SMPROGRAMS\CpuMeter\CpuMeter.lnk" "$INSTDIR\CpuMeter.exe" "" "$INSTDIR\CpuMeter.exe"  0
  CreateShortCut  "$SMPROGRAMS\CpuMeter\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
SectionEnd

Section "Shortcut in Startup folder"
  CreateShortCut  "$SMSTARTUP\CpuMeter.lnk" "$INSTDIR\CpuMeter.exe" "" "$INSTDIR\CpuMeter.exe"  0
SectionEnd

Section "Run CpuMeter after install"
  Exec '$INSTDIR\CpuMeter.exe'
SectionEnd

;-------------------------------------------------------------------------------
Section "Uninstall"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CpuMeter"
  ExecWait '"taskkill.exe" /F /IM CpuMeter.exe'
  DeleteRegKey HKCU "Software\vb\CpuMeter"
  Delete "$INSTDIR\CpuMeter.exe"
  Delete "$INSTDIR\uninstall.exe"
  Delete "$SMPROGRAMS\CpuMeter\*.*"
  RMDir  "$SMPROGRAMS\CpuMeter"
  Delete "$INSTDIR\*.*"
  RMDir  "$INSTDIR"
SectionEnd

