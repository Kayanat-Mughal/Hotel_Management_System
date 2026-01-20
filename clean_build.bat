@echo off
REM Clean and rebuild Hotel Management System
echo.
echo ========================================
echo HOTEL MANAGEMENT SYSTEM - CLEAN BUILD
echo ========================================
echo.

REM Remove old build artifacts
if exist "*.obj" del /q "*.obj"
if exist "*.exe" del /q "*.exe"
if exist "*.ilk" del /q "*.ilk"
if exist "*.pdb" del /q "*.pdb"

echo Compilation started at: %date% %time%
echo.

REM Compile with MSVC
REM Using /I for include directories, /EHsc for exception handling
cl.exe ^
  /std:c++latest ^
  /EHsc ^
  /W3 ^
  /I. ^
  /I./include ^
  /O2 ^
  /D_CRT_SECURE_NO_WARNINGS ^
  src/Utils.cpp ^
  src/Room.cpp ^
  src/Customer.cpp ^
  src/Employee.cpp ^
  src/Reservation.cpp ^
  src/Bill.cpp ^
  src/Database.cpp ^
  src/Hotel.cpp ^
  src/main.cpp ^
  /Fe:hotel_system.exe

if %errorlevel% equ 0 (
    echo.
    echo ========================================
    echo BUILD SUCCESSFUL!
    echo ========================================
    echo.
    echo Executable: hotel_system.exe
    echo.
    dir hotel_system.exe
) else (
    echo.
    echo ========================================
    echo BUILD FAILED!
    echo ========================================
    echo Error Level: %errorlevel%
)

echo.
pause
