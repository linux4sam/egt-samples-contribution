#---- Sliderb
if(EGT_SAMPLES_CONTRIBUTION_SLIDERB)
    find_package(Threads REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBEGT REQUIRED IMPORTED_TARGET libegt)
    pkg_check_modules(LIBEGT REQUIRED IMPORTED_TARGET cairo)

    link_directories(${LIBEGT_LIBRARY_DIRS})

    add_executable(egt-sliderb main.cpp sliderb.cpp sliderb.h custom_theme.h)

    target_link_libraries(egt-sliderb PkgConfig::LIBEGT ${LIBEGT_LIBRARIES} Threads::Threads)

    install(TARGETS egt-sliderb DESTINATION bin)
endif()
