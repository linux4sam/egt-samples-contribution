#---- Sliderb
if(EGT_SAMPLES_CONTRIBUTION_SLIDERB)
    pkg_check_modules(LIBEGT REQUIRED libegt>=1.11)

    add_executable(egt-sliderb main.cpp)

    target_compile_definitions(egt-sliderb PRIVATE EXAMPLEDATA="${CMAKE_INSTALL_FULL_DATADIR}/egt/samples/sliderb")
    target_link_libraries(egt-sliderb PRIVATE ${LIBEGT_LIBRARIES})

    install(TARGETS egt-sliderb RUNTIME)
    install(DIRECTORY data/
            DESTINATION ${CMAKE_INSTALL_DATADIR}/egt/samples/sliderb
            FILES_MATCHING
            PATTERN *.png
            PATTERN *.xml
    )
endif()
