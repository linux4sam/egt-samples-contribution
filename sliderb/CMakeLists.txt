#---- Sliderb
if(EGT_SAMPLES_CONTRIBUTION_SLIDERB)
    add_executable(egt-sliderb main.cpp sliderb.cpp sliderb.h)

    target_link_libraries(egt-sliderb ${LIBEGT_LIBRARIES} Threads::Threads)

    install(TARGETS egt-sliderb DESTINATION bin)
endif()
