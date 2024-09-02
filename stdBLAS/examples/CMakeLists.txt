cmake_minimum_required(VERSION 3.22)
project(Test
  VERSION 0.0.1
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)

find_package(mdspan REQUIRED)
find_package(LinAlg REQUIRED)

if(NOT TARGET TEST::BLAS)
  add_library(TEST::BLAS INTERFACE IMPORTED GLOBAL)
endif()
target_link_libraries(TEST::BLAS INTERFACE "${BLAS_LIBRARY}")


add_executable(cholesky cholesky.cpp)
target_link_libraries(cholesky std::linalg)

add_executable(gemm_std gemm.cpp)
target_link_libraries(gemm_std std::linalg)

add_executable(gemm1 gemm.cpp)
target_link_libraries(gemm1 std::linalg)
target_compile_definitions(gemm1 PUBLIC GEMM=gemm_1)

add_executable(gemm2 gemm.cpp)
target_link_libraries(gemm2 std::linalg)
target_compile_definitions(gemm2 PUBLIC GEMM=gemm_2)

add_executable(gemm3 gemm.cpp)
target_link_libraries(gemm3 std::linalg)
target_compile_definitions(gemm3 PUBLIC GEMM=gemm_3)

add_executable(gemm gemm.cpp)
target_link_libraries(gemm std::linalg)
target_compile_definitions(gemm PUBLIC GEMM=gemm)

add_executable(gemm_blas gemm.cpp)
target_link_libraries(gemm_blas std::linalg TEST::BLAS)
target_compile_definitions(gemm_blas PUBLIC GEMMBLAS)

add_executable(gemm_std_A gemm.cpp)
target_link_libraries(gemm_std_A std::linalg)
target_compile_definitions(gemm_std_A PUBLIC SCALED_A)

add_executable(gemm1_A gemm.cpp)
target_link_libraries(gemm1_A std::linalg)
target_compile_definitions(gemm1_A PUBLIC GEMM=gemm_1 SCALED_A)

add_executable(gemm2_A gemm.cpp)
target_link_libraries(gemm2_A std::linalg)
target_compile_definitions(gemm2_A PUBLIC GEMM=gemm_2 SCALED_A)

add_executable(gemm3_A gemm.cpp)
target_link_libraries(gemm3_A std::linalg)
target_compile_definitions(gemm3_A PUBLIC GEMM=gemm_3 SCALED_A)

add_executable(gemm_A gemm.cpp)
target_link_libraries(gemm_A std::linalg)
target_compile_definitions(gemm_A PUBLIC GEMM=gemm SCALED_A)

add_executable(gemm_blas_A gemm.cpp)
target_link_libraries(gemm_blas_A std::linalg TEST::BLAS)
target_compile_definitions(gemm_blas_A PUBLIC GEMMBLAS SCALED_A)

add_executable(gemm_std_B gemm.cpp)
target_link_libraries(gemm_std_B std::linalg)
target_compile_definitions(gemm_std_B PUBLIC SCALED_B)

add_executable(gemm1_B gemm.cpp)
target_link_libraries(gemm1_B std::linalg)
target_compile_definitions(gemm1_B PUBLIC GEMM=gemm_1 SCALED_B)

add_executable(gemm2_B gemm.cpp)
target_link_libraries(gemm2_B std::linalg)
target_compile_definitions(gemm2_B PUBLIC GEMM=gemm_2 SCALED_B)

add_executable(gemm3_B gemm.cpp)
target_link_libraries(gemm3_B std::linalg)
target_compile_definitions(gemm3_B PUBLIC GEMM=gemm_3 SCALED_B)

add_executable(gemm_B gemm.cpp)
target_link_libraries(gemm_B std::linalg)
target_compile_definitions(gemm_B PUBLIC GEMM=gemm SCALED_B)

add_executable(gemm_blas_B gemm.cpp)
target_link_libraries(gemm_blas_B std::linalg TEST::BLAS)
target_compile_definitions(gemm_blas_B PUBLIC GEMMBLAS SCALED_B)
