REM Build script for game engine
@ECHO OFF

:: Set the timestamp variable
for /f "delims=" %%a in ('powershell -command "[datetime]::UtcNow.ToString('yyyyMMddHHmmss')"') do set "timestamp=%%a"

SET libs=-luser32 -lopengl32 -lgdi32
SET warnings=-Wno-writable-strings -Wno-format-security -Wno-deprecated-declarations -Wno-switch
SET includes=-Ivendors -Ivendors/Include

clang++ %includes% -g src/main.cpp -o puzzle.exe %libs% %warnings%

:: Remove old game_* files if they exist
del game_*

:: Compile game.cpp as a shared library
clang++ -g src/game.cpp -shared -o game_%timestamp%.dll %warnings%

:: Rename the generated DLL
copy game_%timestamp%.dll game.dll