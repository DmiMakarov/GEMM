#ifndef NAIVE_GEMM_H
#define NAIVE_GEMM_H
#include "matrix.h"


/**@brief Multiply a matrix by a scalar
 *
 * @param alpha Scalar multiplier
 * @param A Pointer to the matrix
 * @param B Pointer to the result matrix
 */

void naive_matrix_num_mult(double alpha, struct matrix* A, struct matrix* B);

/**@brief Multiply two matrices
 *
 * @param A Pointer to the first matrix
 * @param B Pointer to the second matrix
 * @param C Pointer to the result matrix
 */
int naive_matrix_matrix_mult(struct matrix* A, struct matrix* B, struct matrix* C);
/**@brief Add two matrices
 *
 * @param A Pointer to the first matrix
 * @param B Pointer to the second matrix
 * @param C Pointer to the result matrix
 */
int naive_matrix_matrix_add(struct matrix* A, struct matrix* B, struct matrix* C);

/**@brief Naive GEMM implementation a*A*B + b*C
 *
 * @param alpha Scalar multiplier for A*B
 * @param beta Scalar multiplier for C
 * @param A Pointer to the first matrix
 * @param B Pointer to the second matrix
 * @param C Pointer to the accumulator matrix
 * @param D Pointer to the result matrix
 */
int naive_gemm(double alpha, double beta, struct matrix* A, struct matrix* B, struct matrix* C, struct matrix* D);


#endif // NAIVE_GEMM_H
