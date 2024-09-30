cmake_minimum_required(VERSION 3.24)

project(cpp_course_2024)

#------------------------------------------------------------------------------
# Setup Output directories
#------------------------------------------------------------------------------

option(CPP_COURSE_BUILD_SLIDES "Whether to build slides." ON)
option(CPP_COURSE_BUILD_SLIDES_PDF "Whether to build slides." ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)

if (${CPP_COURSE_BUILD_SLIDES})
    include(../slides-support/Marp.cmake)
    add_marp_slides(linaro_forge_slides linaro_forge.md)
endif()
