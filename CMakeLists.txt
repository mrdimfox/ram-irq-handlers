# ramisr library
# 
# Description:
#  This library provides some tools for a better integration
#  ISR in your modern C++, but it assumes that vectors table
#  located in RAM. See more in documentation.
# 
# CMake:
#  Use `add_subfolder` to add the library into your code. After
#  that `ramisr` interface library will be generated and you can
#  link it to any of your targets.
# 
# Requirements:
#  This library requaires at least `cxx_std_17` feature support.
#

cmake_minimum_required(VERSION 3.13)

project(ramisr)

add_library(${PROJECT_NAME} INTERFACE)

# Add better compile commands support
target_sources(${PROJECT_NAME}
    INTERFACE
        ${PROJECT_SOURCE_DIR}/src/ramisr/ports/opencm3.hpp
        ${PROJECT_SOURCE_DIR}/src/ramisr/isr.hpp
)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        ${PROJECT_SOURCE_DIR}/src
)

target_compile_features(${PROJECT_NAME}
    INTERFACE
        cxx_std_17
)

#--> Examples building
    # Enable this option using cmake-gui and then reconfigure the project from there
    option(RAMISR_BUILD_EXAMPLES "Build ramisr library examples" OFF)

    # Built `examples` target will be located in "build/examples" folder
    if (RAMISR_BUILD_EXAMPLES)
        add_subdirectory(examples)
    endif()
# <--