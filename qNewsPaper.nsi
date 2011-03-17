!include "MUI2.nsh"
!include "UAC.nsh"

;General
  ;Name and file
  Name "qNewsPaper"
  OutFile "qNewsPaper.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\qNewsPaper"

  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\qNewsPaper" ""

  ;Request application privileges for Windows 7/Vista
  RequestExecutionLevel user

  ;Attempt to give the UAC plug-in a user process and an admin process.
  Function .OnInit

  UAC_Elevate:
  !insertmacro UAC_RunElevated 
  StrCmp 1223 $0 UAC_ElevationAborted ; UAC dialog aborted by user?
  StrCmp 0 $0 0 UAC_Err ; Error?
  StrCmp 1 $1 0 UAC_Success ;Are we the real deal or just the wrapper?
  Quit

  UAC_Err:
  MessageBox mb_iconstop "Unable to elevate, error $0"
  Abort

  UAC_ElevationAborted:
  #elevation was aborted, run as normal?
  MessageBox mb_iconstop "This installer requires admin access, aborting!"
  Abort

  UAC_Success:
  StrCmp 1 $3 +4 ;Admin?
  StrCmp 3 $1 0 UAC_ElevationAborted ;Try again?
  MessageBox mb_iconstop "This installer requires admin access, try again"
  goto UAC_Elevate 

  FunctionEnd

;Variables
  Var StartMenuFolder

;Interface Settings
  !define MUI_ABORTWARNING

;Pages
  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "C:\Users\Hakim\Desktop\files\qNewsPaper\License.txt"
  !insertmacro MUI_PAGE_DIRECTORY

  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\qNewsPaper"
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "qNewsPaper"

  !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder

  !insertmacro MUI_PAGE_INSTFILES

  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;Languages
  !insertmacro MUI_LANGUAGE "English"

;Installer Sections
Section "Install" qNewsPaper

  SetOutPath "$INSTDIR"

  ;ADD YOUR OWN FILES HERE...
  File "C:\Users\Hakim\Desktop\files\qNewsPaper\License.txt"
  File "C:\Users\Hakim\Desktop\files\qNewsPaper\Changelog.txt"
  File "C:\Users\Hakim\Desktop\files\qNewsPaper\qNewsPaper.exe"
  File "C:\Users\Hakim\Desktop\files\qNewsPaper\libgcc_s_dw2-1.dll"
  File "C:\Users\Hakim\Desktop\files\qNewsPaper\mingwm10.dll"
  File "C:\Users\Hakim\Desktop\files\qNewsPaper\QtCore4.dll"
  File "C:\Users\Hakim\Desktop\files\qNewsPaper\QtGui4.dll"
  File "C:\Users\Hakim\Desktop\files\qNewsPaper\QtNetwork4.dll"
  File "C:\Users\Hakim\Desktop\files\qNewsPaper\QtXml4.dll"
  File "C:\Users\Hakim\Desktop\files\qNewsPaper\qnewspaper_en.qm"
  File "C:\Users\Hakim\Desktop\files\qNewsPaper\qnewspaper_fr.qm"

  ;Store installation folder
  WriteRegStr HKCU "Software\qNewsPaper" "" $INSTDIR

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application

  ;Create shortcuts
  CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
  CreateShortCut "$SMPROGRAMS\$StartMenuFolder\qNewsPaper.lnk" "$INSTDIR\qNewsPaper.exe"
  CreateShortCut "$DESKTOP\qNewsPaper.lnk" "$INSTDIR\qNewsPaper.exe"
  CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"

  !insertmacro MUI_STARTMENU_WRITE_END

  WriteRegStr HKLM "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\layers" \
  "$INSTDIR\qNewsPaper.exe" "RUNASADMIN"
  WriteRegStr HKLM "Software\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\layers" \
  "$INSTDIR\Uninstall.exe" "RUNASADMIN"

SectionEnd

;Uninstaller Section
Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...
  Delete "$INSTDIR\qNewsPaper.exe"
  Delete "$INSTDIR\License.txt"
  Delete "$INSTDIR\Changelog.txt"
  Delete "$INSTDIR\libgcc_s_dw2-1.dll"
  Delete "$INSTDIR\mingwm10.dll"
  Delete "$INSTDIR\QtCore4.dll"
  Delete "$INSTDIR\QtGui4.dll"
  Delete "$INSTDIR\QtNetwork4.dll"
  Delete "$INSTDIR\QtXml4.dll"
  Delete "$INSTDIR\qnewspaper_en.qm"
  Delete "$INSTDIR\qnewspaper_fr.qm"
  Delete "$INSTDIR\newspapers.xml"
  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR"

  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder

  Delete "$SMPROGRAMS\$StartMenuFolder\qNewsPaper.lnk"
  Delete "$DESKTOP\qNewsPaper.lnk" 
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"

  DeleteRegKey /ifempty HKCU "Software\qNewsPaper"
  ;DeleteRegKey RUNASADMIN !

SectionEnd
