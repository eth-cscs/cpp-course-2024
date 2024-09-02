cmake_minimum_required(VERSION 3.24)

project(cpp_course_2024 CXX CUDA)

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

if (${CPP_COURSE_BUILD_SLIDES})
    include(../slides-support/Marp.cmake)
endif()

include(CTest)


if (${CPP_COURSE_BUILD_SLIDES})
    add_marp_slides(mdspan_slides mdspan.md)
endif()

if (${CPP_COURSE_BUILD_CODE})
    include(FetchContent)
    FetchContent_Declare(
        mdspan
        GIT_REPOSITORY https://github.com/kokkos/mdspan.git
        GIT_TAG        stable
    )
    FetchContent_MakeAvailable(mdspan)

    add_executable(mdspan_example code/mdspan.cpp)
    target_link_libraries(mdspan_example mdspan)
    target_compile_features(mdspan_example PRIVATE cxx_std_23)

    add_test(NAME mdspan_example COMMAND mdspan_example)


    add_executable(mdarray_example code/mdarray.cpp)
    target_link_libraries(mdarray_example mdspan)
    target_compile_features(mdarray_example PRIVATE cxx_std_23)

    add_test(NAME mdarray_example COMMAND mdarray_example)
    
    add_executable(submdspan_example code/submdspan.cpp)
    target_link_libraries(submdspan_example mdspan)
    target_compile_features(submdspan_example PRIVATE cxx_std_23)

    add_test(NAME submdspan_example COMMAND submdspan_example)
endif()
