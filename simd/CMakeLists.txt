cmake_minimum_required(VERSION 3.24)

project(cpp_course_2024 CXX)

#------------------------------------------------------------------------------
# Setup Output directories
#------------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all Executables."
)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/lib CACHE PATH "Single Directory for all Libraries"
)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/lib CACHE PATH "Single Directory for all static libraries."
)

option(CPP_COURSE_BUILD_SLIDES "Whether to build slides." ON)
option(CPP_COURSE_BUILD_SLIDES_PDF "Whether to build slides." ON)
option(CPP_COURSE_BUILD_CODE "Whether to build code samples." ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)

if (${CPP_COURSE_BUILD_SLIDES})
    include(../slides-support/Marp.cmake)
    add_marp_slides(simd_slides simd.md)
endif()

if (${CPP_COURSE_BUILD_CODE})
    add_executable(simd code.cpp)
endif()
