cmake_minimum_required(VERSION 3.14)
project(GEMM C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

# All sources live under src/; headers included as "matrix.h", "naive_gemm.h", etc.
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# ---- Test executables (each has its own main) ----

add_executable(test_matrix
    ${TEST_DIR}/test_matrix.c
    ${SRC_DIR}/matrix.c
)
target_include_directories(test_matrix PRIVATE ${SRC_DIR})

add_executable(test_naive
    ${TEST_DIR}/test_naive.c
    ${SRC_DIR}/matrix.c
    ${SRC_DIR}/naive_gemm.c
)
target_include_directories(test_naive PRIVATE ${SRC_DIR})

add_executable(test_transpose_gemm
    ${TEST_DIR}/test_transpose_gemm.c
    ${SRC_DIR}/matrix.c
    ${SRC_DIR}/transpose_gemm.c
)
target_include_directories(test_transpose_gemm PRIVATE ${SRC_DIR})

# ---- Comparison program ----

add_executable(comparison
    ${SRC_DIR}/comparison.c
    ${SRC_DIR}/matrix.c
    ${SRC_DIR}/naive_gemm.c
    ${SRC_DIR}/transpose_gemm.c
)
target_link_libraries(comparison PRIVATE m)
target_include_directories(comparison PRIVATE ${SRC_DIR})


# ---- CTest: run one test or all tests ----

enable_testing()

add_test(NAME test_matrix      COMMAND test_matrix)
add_test(NAME test_naive       COMMAND test_naive)
add_test(NAME test_transpose   COMMAND test_transpose_gemm)
