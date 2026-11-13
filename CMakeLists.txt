cmake_minimum_required(VERSION 3.24)
project(Ricochet VERSION 0.1.0 LANGUAGES C CXX)

include(FetchContent)

# Build SDL with the same compiler as the game (no manual SDK installation).
set(SDL_SHARED ON CACHE BOOL "Build SDL shared library" FORCE)
set(SDL_STATIC OFF CACHE BOOL "Build SDL static library" FORCE)
set(SDL_TESTS OFF CACHE BOOL "Build SDL tests" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "Build SDL test library" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "Build SDL examples" FORCE)
FetchContent_Declare(SDL3
    URL https://github.com/libsdl-org/SDL/releases/download/release-3.4.0/SDL3-3.4.0.tar.gz
    URL_HASH SHA256=082cbf5f429e0d80820f68dc2b507a94d4cc1b4e70817b119bbb8ec6a69584b8
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(SDL3)

add_executable(Ricochet src/main.cpp)
target_compile_features(Ricochet PRIVATE cxx_std_20)
set_target_properties(Ricochet PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(Ricochet PRIVATE SDL3::SDL3)

if(MSVC)
    target_compile_options(Ricochet PRIVATE /W4)
else()
    target_compile_options(Ricochet PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Windows needs SDL3.dll next to the executable, including when run from CLion.
if(WIN32)
    add_custom_command(TARGET Ricochet POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_FILE:SDL3::SDL3>" "$<TARGET_FILE_DIR:Ricochet>"
        VERBATIM
    )
endif()
