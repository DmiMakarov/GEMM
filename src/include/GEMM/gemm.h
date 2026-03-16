#ifndef GEMM_H
#define GEMM_H

#include "matrix.h"


enum gemm_type {
    GEMM_NAIVE,
    GEMM_TRANSPOSE,
    GEMM_WINOGRAD,
};
/**@brief GEMM implementation
 *
 * @param gemm_type Type of GEMM implementation
 * @param alpha Scalar multiplier for A*B
 * @param beta Scalar multiplier for C
 * @param A Pointer to the first matrix
 * @param B Pointer to the second matrix
 * @param C Pointer to the accumulator matrix
 * @param D Pointer to the result matrix
 * @return 0 if successful, 1 if there was an error
 */
int gemm(enum gemm_type gemm_type, double alpha, double beta, struct matrix* A, struct matrix* B, struct matrix* C, struct matrix* D);

/**@brief Default GEMM implementation a*A*B + b*C with alpha = beta = 1.0
 *
 * @param gemm_type Type of GEMM implementation
 * @param A Pointer to the first matrix
 * @param B Pointer to the second matrix
 * @param C Pointer to the accumulator matrix
 * @param D Pointer to the result matrix
 * @return 0 if successful, 1 if there was an error
 */
int gemm_default(enum gemm_type gemm_type, struct matrix* A, struct matrix* B, struct matrix* C, struct matrix* D);
#endif // GEMM_H
