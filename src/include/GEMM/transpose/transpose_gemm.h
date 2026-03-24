#ifndef TRANSPOSE_GEMM_H
#define TRANSPOSE_GEMM_H
#include "matrix.h"

/**@brief Naive GEMM implementation a*A*B + b*C
 *
 * @param alpha Scalar multiplier for A*B
 * @param beta Scalar multiplier for C
 * @param A Pointer to the first matrix
 * @param B Pointer to the second matrix
 * @param C Pointer to the accumulator matrix
 * @param D Pointer to the result matrix
 */
int transpose_gemm(double alpha, double beta, struct matrix* A, struct matrix* B, struct matrix* C, struct matrix* D);


#endif // TRANSPOSE_GEMM_H
