@echo off

for /f "skip=1delims=" %%a in (
 'wmic process where "name='FnHotkeyUtility.exe'" get ExecutablePath'
) do set "exepath=%%a"&goto next
:next

taskkill /IM FnHotkeyUtility.exe /F

start "" "%exepath%"