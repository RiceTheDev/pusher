cmake_minimum_required(VERSION 3.16)

project(bytepusher LANGUAGES C CXX)

include(FetchContent)

FetchContent_Declare(
    nfd
    GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git
    GIT_TAG v1.3.0
)

set(NFD_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(NFD_PORTAL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(nfd)

if(WIN32)

    if(MSVC)
        set(SDL2_ROOT
            "${CMAKE_CURRENT_SOURCE_DIR}/scripts/msvc64_libs"
        )
    elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        set(SDL2_ROOT
            "${CMAKE_CURRENT_SOURCE_DIR}/scripts/mingw64_libs"
        )
    else()
        message(FATAL_ERROR
            "Unsupported Windows compiler: ${CMAKE_C_COMPILER_ID}"
        )
    endif()
else()
    find_package(SDL2 REQUIRED)
endif()


if(WIN32)

    set(SDL2_DIR
        "${SDL2_ROOT}/lib/cmake/SDL2"
        CACHE PATH "SDL2 CMake package directory"
        FORCE
    )

    set(CMAKE_PREFIX_PATH
        "${SDL2_ROOT}"
        CACHE STRING "CMake package search path"
        FORCE
    )

    find_package(SDL2 CONFIG REQUIRED)

    message(STATUS "Using SDL2 from: ${SDL2_ROOT}")

endif()

set(IMGUI_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/source/ui/imgui"
)

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
    source/input.c
    source/rom.c
    source/audio.c

    source/screen.cpp
    source/ui/ui.cpp
    source/ui/theme.cpp
    source/assets.cpp

    ${IMGUI_SOURCES}
)

target_include_directories(bytepusher PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/source/miniaudio"

    "${IMGUI_DIR}"
    "${IMGUI_DIR}/backends"

    "${nfd_SOURCE_DIR}/src/include"
)

if(WIN32)

    target_include_directories(bytepusher PRIVATE
        "${SDL2_ROOT}/include"
    )

endif()

target_link_libraries(bytepusher PRIVATE

    SDL2::SDL2main
    SDL2::SDL2
    nfd

)

set_property(TARGET bytepusher PROPERTY C_STANDARD 11)
set_property(TARGET bytepusher PROPERTY C_STANDARD_REQUIRED ON)

set_property(TARGET bytepusher PROPERTY CXX_STANDARD 17)
set_property(TARGET bytepusher PROPERTY CXX_STANDARD_REQUIRED ON)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_link_options(bytepusher PRIVATE
        -static-libgcc
        -static-libstdc++
    )
endif()