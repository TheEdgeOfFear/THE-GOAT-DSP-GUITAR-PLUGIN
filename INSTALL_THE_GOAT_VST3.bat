@echo off
setlocal enabledelayedexpansion
echo =========================================================================
echo  THE GOAT - VST3 System Installer / Updater
echo  THE EDGE OF FEAR - HM-2 Cult Chainsaw DSP Guitar Plugin
echo =========================================================================
echo.

net session >nul 2>&1
if %errorlevel% neq 0 (
    echo Administrator privileges required to install to C:\Program Files\Common Files\VST3.
    echo Requesting administrator elevation...
    powershell -NoProfile -ExecutionPolicy Bypass -Command "Start-Process cmd -ArgumentList '/c \"\"%~f0\"\"' -Verb RunAs"
    exit /b
)

set "SRC_DIR="
if exist "%~dp0THE GOAT.vst3\Contents\x86_64-win\THE GOAT.vst3" (
    set "SRC_DIR=%~dp0THE GOAT.vst3"
) else if exist "%~dp0VST3\THE GOAT.vst3\Contents\x86_64-win\THE GOAT.vst3" (
    set "SRC_DIR=%~dp0VST3\THE GOAT.vst3"
) else if exist "%~dp0..\VST3\THE GOAT.vst3\Contents\x86_64-win\THE GOAT.vst3" (
    set "SRC_DIR=%~dp0..\VST3\THE GOAT.vst3"
)

if "%SRC_DIR%"=="" (
    echo [ERROR] Could not locate THE GOAT.vst3 bundle folder!
    echo Please make sure the VST3 folder is present.
    pause
    exit /b 1
)

echo Found VST3 bundle at: %SRC_DIR%
echo.
echo Cleaning any existing installation from C:\Program Files\Common Files\VST3\THE GOAT.vst3...
if exist "C:\Program Files\Common Files\VST3\THE GOAT.vst3" (
    rd /s /q "C:\Program Files\Common Files\VST3\THE GOAT.vst3"
)

echo Deploying THE GOAT.vst3 to system VST3 directory...
if not exist "C:\Program Files\Common Files\VST3" (
    mkdir "C:\Program Files\Common Files\VST3"
)

robocopy "%SRC_DIR%" "C:\Program Files\Common Files\VST3\THE GOAT.vst3" /E /IS /IT /NFL /NDL

if exist "C:\Program Files\Common Files\VST3\THE GOAT.vst3\Contents\x86_64-win\THE GOAT.vst3" (
    echo.
    echo =========================================================================
    echo  [SUCCESS] THE GOAT.vst3 successfully installed!
    echo  Location: C:\Program Files\Common Files\VST3\THE GOAT.vst3
    echo =========================================================================
    echo Please restart or rescan plugins in your DAW (Reaper, Cubase, Ableton, FL Studio, etc.).
) else (
    echo.
    echo [ERROR] Installation verification failed.
)

echo.
pause
