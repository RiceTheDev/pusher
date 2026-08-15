![logo](https://github.com/user-attachments/assets/067694a3-6344-4eed-9624-160a973a414a)

# pusher

<p align="center">
  
[![Github Repository](https://img.shields.io/badge/github-repo-blue?logo=github)](https://github.com/ricethedev/pusher)
![GitHub commit activity](https://img.shields.io/github/commit-activity/t/RiceTheDev/pusher)
[![MIT License](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/)

</p>

a simple [bytepusher](https://esolangs.org/wiki/BytePusher) emulator with various debug options.

## screenshots
### animation demo
<img width="642" height="351" alt="Screenshot_2026-08-15_14-02-04" src="https://github.com/user-attachments/assets/139ce1f2-bb46-4146-af31-b4b9c974e1b5" />

### audio test
<img width="642" height="351" alt="Screenshot_2026-08-15_14-02-04" src="https://github.com/user-attachments/assets/4c884e60-0224-4cd5-817b-18f8a517861d" />

### memory editing
<img width="642" height="351" alt="Screenshot_2026-08-15_14-02-04" src="https://github.com/user-attachments/assets/0da1dff5-2659-4b2d-86fc-876cda4d5ee6" />

## downloading

pusher has prebuild binaries for windows and linux, you can check on the [releases](https://github.com/RiceTheDev/pusher/releases) page on github.

## building

### (for cross-compilling on linux, check cmake/readme.md)

#### - clone the project

```bash
  git clone https://github.com/RiceTheDev/pusher
```

#### - go to the project directory

```bash
  cd pusher
```

#### - enter the "scripts/" directory

```bash
  cd scripts
```

#### - run the build script for your system

```bash
  # linux (gcc)
  chmod +x build-linux.sh
  ./build-linux.sh

  # windows (mingw)
  chmod +x build-mingw64.sh
  ./build-mingw64.sh

  # windows (msvc/visual studio)
  # open cmd.exe on scripts/
  build-win64.bat
```

if everything goes correctly, you should have an executable! <br>
the script gives you the path to your compilled executable.

## support

for support, you can send me a dm on discord at @raic.e or open a issue on here!