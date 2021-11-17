cmake_minimum_required(VERSION 3.12)

project(
    egt_samples_contribution
    VERSION 1.0.0
    DESCRIPTION "A collection of samples made by the community"
    LANGUAGES CXX)

option(EGT_SAMPLES_CONTRIBUTION_SLIDERB, FALSE)
if(EGT_SAMPLES_CONTRIBUTION_SLIDERB)
  add_subdirectory(sliderb)
endif()

