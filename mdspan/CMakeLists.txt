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

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (${CPP_COURSE_BUILD_SLIDES})
    include(../slides-support/Marp.cmake)
endif()

include(CTest)


if (${CPP_COURSE_BUILD_SLIDES})
    add_marp_slides(mdspan mdspan.md)
endif()

if (${CPP_COURSE_BUILD_CODE})
    include(FetchContent)
    FetchContent_Declare(
        mdspan
        GIT_REPOSITORY https://github.com/kokkos/mdspan.git
        GIT_TAG        stable
    )
    FetchContent_MakeAvailable(mdspan)

    add_executable(03_02b_mdspan code/mdspan.cpp)
    target_link_libraries(03_02b_mdspan mdspan)
    target_compile_features(03_02b_mdspan PRIVATE cxx_std_23)

    add_test(NAME 03_02b_mdspan COMMAND 03_02b_mdspan)
endif()
