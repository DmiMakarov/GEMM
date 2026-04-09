#ifndef STRASSEN_GEMM_H
#define STRASSEN_GEMM_H
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
int strassen_gemm(double alpha, double beta, struct matrix* A, struct matrix* B, struct matrix* C, struct matrix* D);

int strassen_matrix_num_mult(double alpha, struct matrix* A, struct matrix* B);
int strassen_matrix_matrix_mult(struct matrix* A, struct matrix* B, struct matrix* C);
int strassen_matrix_matrix_add(struct matrix* A, struct matrix* B, struct matrix* C);

#endif // STRASSEN_GEMM_H
