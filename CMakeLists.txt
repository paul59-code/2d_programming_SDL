cmake_minimum_required(VERSION 3.20)
project(proba_2d_programming)

set(CMAKE_CXX_STANDARD 17)

# Indică noua cale către folderul SDL
# (Dacă folderul se numește "SDL3", modifică linia de mai jos în add_subdirectory(vendored/SDL3))
add_subdirectory(vendored/SDL)

add_executable(proba_2d_programming main.cpp
        Box.cpp)

target_link_libraries(proba_2d_programming PRIVATE SDL3::SDL3)

include_directories(${CMAKE_SOURCE_DIR})