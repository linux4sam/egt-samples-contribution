cmake_minimum_required(VERSION 3.12)

project(
    egt_samples_contribution
    VERSION 1.2.1
    DESCRIPTION "A collection of samples made by the community"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBEGT libegt)

include_directories(${LIBEGT_INCLUDE_DIRS})
link_directories(${LIBEGT_LIBRARY_DIRS})

option(EGT_SAMPLES_CONTRIBUTION_SLIDERB, FALSE)
if(EGT_SAMPLES_CONTRIBUTION_SLIDERB)
  add_subdirectory(sliderb)
endif()

