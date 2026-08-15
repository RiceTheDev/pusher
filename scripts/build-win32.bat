@echo off

cd ..

if not exist build mkdir build

cmake -S . -B build -A x64
if errorlevel 1 pause & exit /b 1

cmake --build build --config Release
if errorlevel 1 pause & exit /b 1

echo.
echo build complete!
echo executable: build\Release\bytepusher.exe

pause
