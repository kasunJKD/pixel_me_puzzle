#!/bin/bash

timestamp=$(date +%s)

warnings="-Wno-writable-strings -Wno-format-security -Wno-deprecated-declarations -Wno-switch"
includes="-Ithird_party -Ithird_party/Include"

# Replace Windows-specific libraries with their Linux equivalents
libs="-lX11 -lGL -lGLU"

# Build the executable for Linux
g++ $includes -g src/main.cpp -o schnitzel $libs $warnings

# Remove old game_* files
rm -f game_*

# Build the shared library for Linux
g++ -shared -fPIC -g src/game.cpp -o game_$timestamp.so $warnings
mv game_$timestamp.so game.so
