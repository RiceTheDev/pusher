# hey!

### this is only for those who want to cross-compile from linux -> windows.
### steps:
#### - install the following package

debian based systems (ubuntu, pop os, etc):
```bash
sudo apt-get install mingw-w64
```

arch based systems (cachy, steamos, endeavour):
```bash
sudo pacman -S mingw-w64-gcc mingw-w64-crt mingw-w64-headers mingw-w64-winpthreads
```

fedora based systems (i dont know any):
```bash
sudo dnf install mingw64-gcc-c++
```

<br>

#### - building

now, we just need to build!

```bash
cd ..
mkdir build
cmake -S . -DCMAKE_TOOLCHAIN_FILE=cmake/mingw-w64.cmake
cmake -B build
```

and now... you should have a windows executable for pusher!<br>
you might need to pack it with sdl2.dll, you can find it on `scripts/mingw64_libs/bin/sdl2.dll`