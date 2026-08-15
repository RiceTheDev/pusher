cmake_minimum_required(VERSION 3.10)

project(bytepusher LANGUAGES C CXX)

include(FetchContent)

FetchContent_Declare(
    nfd
    GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git
    GIT_TAG        v1.3.0
)

set(NFD_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(NFD_PORTAL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(nfd)

if(WIN32)
    if(MSVC)
        set(SDL2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/scripts/msvc64_libs/lib/cmake/SDL2")
    else()
        set(SDL2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/scripts/mingw64_libs/lib/cmake/SDL2")
    endif()
endif()
find_package(SDL2 REQUIRED)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source/ui/imgui)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp

    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer2.cpp
)

add_executable(bytepusher

    source/main.c

    source/cpu.c
    source/screen.cpp
    source/input.c
    source/rom.c
    source/audio.c
    source/ui/ui.cpp
    source/ui/theme.cpp
    source/assets.cpp

    ${IMGUI_SOURCES}
)


target_include_directories(bytepusher PRIVATE
    source/miniaudio
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${nfd_SOURCE_DIR}/src/include
)

if (WIN32)
    if(MSVC)
        target_include_directories(bytepusher PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/scripts/msvc64_libs/include)
    else()
        target_include_directories(bytepusher PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/scripts/mingw64_libs/include)
    endif()
endif()


target_link_libraries(bytepusher PRIVATE
    SDL2::SDL2main
    SDL2::SDL2
    nfd
)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_link_options(bytepusher PRIVATE
        -static-libgcc
        -static-libstdc++
    )
endif()