#***************************************************
#
# NSIS Script File
# Generated by Manav's NSIS Wizard
#
# (C)2001-2005 Manav Rathi [manavmax@rediffmail.com]
#
#***************************************************

# [Installer Attributes]
Name "Zino"
OutFile "zinosetup.exe"
Caption "Zino v0.1.x Setup"

# [Licence Attributes]
LicenseText "Zino is free and Open Source software! It is released under the GNU Public License. Please review the license agreement before installing Zino. If you accept all terms of the agreement, click I Agree. "
LicenseData "C:\dev\cpp\zino\license.txt"

# [Additional Installer Settings ]
ShowInstDetails hide
AutoCloseWindow true
SilentInstall normal
CRCCheck on
SetCompress auto
SetDatablockOptimize on
SetOverwrite on

# [Background Gradient]
BGGradient C9D06F CA7135 notext

# [Files]
Section "default"
  SetOutPath $INSTDIR
  File "C:\temp\zino\zino\license.txt"
  File "C:\temp\zino\zino\mingwm10.dll"
  File "C:\temp\zino\zino\QtCore4.dll"
  File "C:\temp\zino\zino\QtGui4.dll"
  File "C:\temp\zino\zino\QtSql4.dll"
  File "C:\temp\zino\zino\zino.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Zino" "DisplayName" "Zino"

  WriteUninstaller "$INSTDIR\uninstall.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Zino" "UninstallString" "$INSTDIR\uninstall.exe"
SectionEnd

# [Directory Selection]
InstallDir "$PROGRAMFILES\Zino"
DirText "Setup will install My Application in the following folder. To install in a different folder, click Browse and select another folder."

# [Shortcuts]
Section "CreateShortCut"
  CreateDirectory "$SMPROGRAMS\Zino"
  SetOutPath $INSTDIR

  CreateShortCut "$SMPROGRAMS\Zino\zino.lnk" "$INSTDIR\zino.exe"
  CreateShortCut "$DESKTOP\zino.lnk" "$INSTDIR\zino.exe"
  CreateShortCut "$QUICKLAUNCH\zino.lnk" "$INSTDIR\zino.exe"
SectionEnd

;called when the installer is nearly finished initializing
Function .onInit
# [show splash screen]
  InitPluginsDir
  SetOutPath $PLUGINSDIR
  File "/oname=$PLUGINSDIR\spltmp.bmp" "C:\dev\cpp\zino\win\zino_logo.bmp"
  Splash::Show "2000" "$PLUGINSDIR\spltmp"
  Delete "$PLUGINSDIR\spltmp.bmp"
FunctionEnd

;called when the install was successful
Function .onInstSuccess
  MessageBox MB_OK "Thanks for installing Zino!"
  Exec "$INSTDIR\zino.exe"
FunctionEnd

# [UnInstallation]
UninstallCaption "Uninstall Zino"
UninstallText "Setup will uninstall Zino from the following folder. To uninstall from a different folder, click Browse and select another folder. Click Uninstall to start the uninstallation."
ShowUninstDetails show

Section "Uninstall"
  Delete "$INSTDIR\uninstall.exe"
  Delete "$INSTDIR\zino.exe"
  Delete "$SMPROGRAMS\Zino\zino.lnk"
  Delete "$DESKTOP\zino.lnk"
  Delete "$QUICKLAUNCH\zino.lnk"
  RMDir "$SMPROGRAMS\Zino"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Zino"
  RMDir $INSTDIR
SectionEnd