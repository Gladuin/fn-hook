@echo off
CD /D %~dp0

NET SESSION >nul 2>&1
IF %ERRORLEVEL% EQU 0 (
    goto :continue
) ELSE (
    ECHO Please run this script as admin. Press any key to exit.
	pause >nul
	exit
)

:continue
REG ADD "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Run" /V "fn-hook injector" /t REG_SZ /F /D "%CD%\injector.exe"

ECHO If you want to move this folder, run remove_from_startup.bat, move the folder, and then run add_to_startup.bat again.
pause