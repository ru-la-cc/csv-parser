@echo off
setlocal enabledelayedexpansion

echo Building csvparser for Windows with MSVC...

REM ビルドディレクトリの作成
if not exist build-msvc mkdir build-msvc
cd build-msvc

REM CMakeの設定（MSVC）
echo Configuring with CMake (MSVC)...
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release
if %ERRORLEVEL% neq 0 (
    echo CMake configuration failed
    exit /b 1
)

REM ビルドの実行
echo Building...
cmake --build . --config Release
if %ERRORLEVEL% neq 0 (
    echo Build failed
    exit /b 1
)

REM テストの実行
echo Running tests...
ctest --output-on-failure
if %ERRORLEVEL% neq 0 (
    echo Tests failed
    exit /b 1
)

echo Build and test completed successfully!
cd .. 