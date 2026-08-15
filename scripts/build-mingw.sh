#!/bin/sh

cd ..

mkdir -p build

cmake -S . -B build

if [ $? -ne 0 ]; then
    exit 1
fi

cmake --build build --config Release

if [ $? -ne 0 ]; then
    exit 1
fi

echo
echo "build completed!"
echo "executable: build/bytepusher"