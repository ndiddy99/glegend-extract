cmake_minimum_required(VERSION 3.18)

project(glegend-extract LANGUAGES C)

set(SOURCE_LIST
    "src/main.c"
    "src/constants.h"
    "src/level.c"
    "src/level.h"
    "src/rom.c"
    "src/rom.h"
    "src/tile.c"
    "src/tile.h"
)

add_executable(glegend-extract ${SOURCE_LIST})

target_include_directories(glegend-extract PRIVATE
    "src"
)

if(MSVC)
    target_compile_options(glegend-extract PRIVATE /W3)
else()
    target_compile_options(glegend-extract PRIVATE -Wall -Wvla -Wformat=2)
endif()

set_target_properties(glegend-extract PROPERTIES
    C_STANDARD 17
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

# work around msvc nonsense
if(MSVC)
    # disable warnings when not using "_s" stdlib functions
    target_compile_definitions(glegend-extract PRIVATE _CRT_SECURE_NO_WARNINGS)
    # read source files as utf-8
    target_compile_options(glegend-extract PRIVATE /utf-8)
    # set the debugger working directory
    set_target_properties(glegend-extract PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()
