@echo off
REM Hotel Management System Build Script for MSVC
echo.
echo ========================================
echo Building Hotel Management System
echo ========================================
echo.

REM Set compiler flags
set COMPILER_FLAGS=/std:c++latest /EHsc /W3 /D_CRT_SECURE_NO_WARNINGS
set INCLUDE_DIRS=/I. /I./include
set SOURCE_FILES=src/Bill.cpp src/Customer.cpp src/Database.cpp src/Employee.cpp src/Hotel.cpp src/main.cpp src/Reservation.cpp src/Room.cpp src/Utils.cpp
set OUTPUT_FILE=hotel_system.exe

echo Compiling with MSVC...
echo.

cl.exe %COMPILER_FLAGS% %INCLUDE_DIRS% %SOURCE_FILES% /Fe%OUTPUT_FILE%

if %errorlevel% neq 0 (
    echo.
    echo ========================================
    echo BUILD FAILED!
    echo ========================================
    echo.
    pause
    exit /b 1
)

echo.
echo ========================================
echo BUILD SUCCESSFUL!
echo ========================================
echo.
echo Executable created: %OUTPUT_FILE%
echo.
echo Running Hotel Management System...
echo.
pause
%OUTPUT_FILE%
