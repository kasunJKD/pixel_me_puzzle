REM Build script for game engine
@ECHO OFF

SET libs=-luser32
SET warnings=-Wno-writable-strings -Wno-format-security -Wno-deprecated-declarations -Wno-switch

clang++ -g src/main.cpp -o ./build/game.exe %libs% %warnings%