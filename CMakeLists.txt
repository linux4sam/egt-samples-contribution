cmake_minimum_required(VERSION 3.12)

project(
    egt_samples_contribution
    VERSION 1.3
    DESCRIPTION "A collection of samples made by the community"
    LANGUAGES CXX)
#set(PROJECT_VERSION_RC 1)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)

include_directories(${LIBEGT_INCLUDE_DIRS})
link_directories(${LIBEGT_LIBRARY_DIRS})

option(EGT_SAMPLES_CONTRIBUTION_SLIDERB, FALSE)
if(EGT_SAMPLES_CONTRIBUTION_SLIDERB)
  add_subdirectory(sliderb)
endif()

