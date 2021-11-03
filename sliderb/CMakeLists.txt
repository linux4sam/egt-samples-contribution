cmake_minimum_required(VERSION 3.14)
project(
    egt-sliderb
    VERSION 0.1.0
    DESCRIPTION "Custom slider"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED true)

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBEGT REQUIRED IMPORTED_TARGET libegt)
pkg_check_modules(LIBEGT REQUIRED IMPORTED_TARGET cairo)

link_directories(${LIBEGT_LIBRARY_DIRS})

add_executable(egt-sliderb main.cpp sliderb.cpp sliderb.h custom_theme.h)

target_link_libraries(egt-sliderb PkgConfig::LIBEGT ${LIBEGT_LIBRARIES} Threads::Threads)

install(TARGETS egt-sliderb DESTINATION bin)
