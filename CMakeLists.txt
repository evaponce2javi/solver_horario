# ⫘⫘⫘ CMakeLists.txt — alternativa a Make ⫘⫘⫘
cmake_minimum_required(VERSION 3.10)
project(HorarioCSP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(horario ${SOURCES})

if(MSVC)
    target_compile_options(horario PRIVATE /W4 /utf-8)
else()
    target_compile_options(horario PRIVATE -Wall -Wextra)
endif()
