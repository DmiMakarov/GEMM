cmake_minimum_required(VERSION 3.14)
project(GEMM C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

# -----------------------------------------------------------------------------
# Source tree: libraries are defined in src/CMakeLists.txt (gemm_matrix,
# gemm_naive, gemm_transpose, gemm). This call runs that file and creates
# those targets in the build.
# -----------------------------------------------------------------------------
add_subdirectory(src)

# -----------------------------------------------------------------------------
# Paths (used for tests and comparison executable which live at top level)
# -----------------------------------------------------------------------------
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set(GEMM_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/src/include)
set(GEMM_HEADERS ${GEMM_INCLUDE}/GEMM)

# ---- Test executables (each has its own main) ----
# Tests use #include "matrix.h" and "naive_gemm.h" / "transpose_gemm.h", so we
# add the GEMM and backend subdirs to the include path.
add_executable(test_matrix ${TEST_DIR}/test_matrix.c)
target_include_directories(test_matrix PRIVATE ${GEMM_HEADERS})
target_link_libraries(test_matrix PRIVATE gemm_matrix)

add_executable(test_naive ${TEST_DIR}/test_naive.c)
target_include_directories(test_naive PRIVATE ${GEMM_HEADERS} ${GEMM_HEADERS}/naive)
target_link_libraries(test_naive PRIVATE gemm_naive)

add_executable(test_transpose_gemm ${TEST_DIR}/test_transpose_gemm.c)
target_include_directories(test_transpose_gemm PRIVATE ${GEMM_HEADERS} ${GEMM_HEADERS}/transpose)
target_link_libraries(test_transpose_gemm PRIVATE gemm_transpose)

# ---- Comparison program (uses dispatcher gemm + math) ----
add_executable(comparison src/comparison.c)
target_include_directories(comparison PRIVATE ${GEMM_INCLUDE})
target_link_libraries(comparison PRIVATE gemm m)

# ---- CTest: run one test or all tests ----
enable_testing()
add_test(NAME test_matrix      COMMAND test_matrix)
add_test(NAME test_naive       COMMAND test_naive)
add_test(NAME test_transpose   COMMAND test_transpose_gemm)
