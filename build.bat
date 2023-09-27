REM Build script for game engine
@ECHO OFF

SET libs=-luser32 -lopengl32 -lgdi32
SET warnings=-Wno-writable-strings -Wno-format-security -Wno-deprecated-declarations -Wno-switch
SET includes=-Ivendors -Ivendors/Include

clang++ %includes% -g src/main.cpp -o ./build/game.exe %libs% %warnings%