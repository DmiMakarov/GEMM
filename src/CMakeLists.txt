# -----------------------------------------------------------------------------
# GEMM source tree: build libraries here. Top-level CMakeLists.txt adds this
# directory with add_subdirectory(src) and then links these targets into
# executables (tests, comparison).
# -----------------------------------------------------------------------------

set(GEMM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(GEMM_HEADERS_DIR ${GEMM_INCLUDE_DIR}/GEMM)

# ---- Core matrix library (init, free, print) ----
add_library(gemm_matrix STATIC matrix.c)
target_include_directories(gemm_matrix PUBLIC ${GEMM_INCLUDE_DIR} ${GEMM_HEADERS_DIR})

# ---- Backend: naive GEMM ----
add_library(gemm_naive STATIC naive/naive_gemm.c)
target_include_directories(gemm_naive PUBLIC ${GEMM_INCLUDE_DIR} ${GEMM_HEADERS_DIR})
target_link_libraries(gemm_naive PUBLIC gemm_matrix)

# ---- Backend: transpose GEMM ----
add_library(gemm_transpose STATIC transpose/transpose_gemm.c)
target_include_directories(gemm_transpose PUBLIC ${GEMM_INCLUDE_DIR} ${GEMM_HEADERS_DIR})
target_link_libraries(gemm_transpose PUBLIC gemm_matrix)

# ---- Dispatcher: gemm.c selects backend by enum ----
add_library(gemm STATIC gemm.c)
target_include_directories(gemm PUBLIC ${GEMM_INCLUDE_DIR} ${GEMM_HEADERS_DIR})
target_link_libraries(gemm PRIVATE gemm_naive gemm_transpose gemm_matrix)
