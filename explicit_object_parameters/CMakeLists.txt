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

option(CPP_COURSE_BUILD_SLIDES "Weather to build slides." OFF)
option(CPP_COURSE_BUILD_SLIDES_PDF "Weather to build slides." ON)
option(CPP_COURSE_BUILD_CODE "Weather to build code samples." OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (${CPP_COURSE_BUILD_SLIDES})
    include(../slides-support/Marp.cmake)
endif()

include(CTest)


if (${CPP_COURSE_BUILD_SLIDES})
    add_marp_slides(explicit_object_parameter explicit_object_parameter.md)
endif()

if (${CPP_COURSE_BUILD_CODE})

    function(add_example name)
        add_executable(${name} ${name}.cpp)
        target_compile_options(${name} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-Wall -Wextra -Wpedantic>)
    endfunction()

    add_example(explicit_object_parameters)
    add_example(derived_types)
    add_example(derived_types_private)
    add_example(crtp)
    add_example(visitor)
    add_example(lambda)
    add_example(tree)
    add_example(factory)
    add_example(member_function_ptr)
    add_example(self_copy_chain)

endif()
