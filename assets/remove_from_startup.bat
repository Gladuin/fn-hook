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
REG DELETE "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Run" /V "fn-hook injector" /f