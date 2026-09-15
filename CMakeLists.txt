cmake_minimum_required(VERSION 3.21)

project(gld DESCRIPTION "Header only OpenGL debugging library." VERSION 2.0.0 LANGUAGES C)

option(GLD_INSTALL "Generate install rules for gld" ${PROJECT_IS_TOP_LEVEL})

add_library(gld INTERFACE)
add_library(jmandevel::gld ALIAS gld)

target_include_directories(
    gld INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>"
)

target_compile_features(gld INTERFACE c_std_99)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

execute_process(
    COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/gen/generate_gld.py"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/gen"
    OUTPUT_VARIABLE GLD_GL_CALLS
    OUTPUT_STRIP_TRAILING_WHITESPACE
    RESULT_VARIABLE GLD_GEN_RESULT
)

if (NOT GLD_GEN_RESULT EQUAL 0)
    message(FATAL_ERROR "generate_gld.py failed")
endif()

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/gen/gld.h.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/gld/gld.h"
    @ONLY
)

set_property(
    DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/gl.xml"
    "${CMAKE_SOURCE_DIR}/gen/generate_gld.py"
)

if(GLD_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    install(
        TARGETS gld 
        EXPORT gldTargets 
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    )

    install(
        DIRECTORY "include/"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    )

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/gldConfigVersion.cmake"
        VERSION 1.0.0
        COMPATIBILITY SameMajorVersion
    )

    install(
        EXPORT gldTargets
        FILE gldTargets.cmake
        NAMESPACE jmandevel::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/gld"
    )

    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/gldConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/gldConfig.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/gld"
    )

    install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/gldConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/gldConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/gld"
    )
endif()