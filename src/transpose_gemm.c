#include <stdio.h>
#include <stdlib.h>
#include "transpose_gemm.h"


/**@brief Multiply a matrix by a scalar
 *
 * @param alpha Scalar multiplier
 * @param A Pointer to the matrix
 * @param B Pointer to the result matrix
 */

void transpose_matrix_num_mult(double alpha, struct matrix* A, struct matrix* B)
{
    for (size_t i = 0; i < A->rows; i++)
    {
        for (size_t j = 0; j < A->cols; j++)
        {
            B->data[i][j] = alpha * A->data[i][j];
        }
    }
}

/**@brief Transpose a matrix
 *
 * @param A Pointer to the matrix
 * @param B Pointer to the result matrix
 */
void transpose_matrix(struct matrix* A, struct matrix* B)
{
    for (size_t i = 0; i < A->rows; i++)
    {
        for (size_t j = 0; j < A->cols; j++)
        {
            B->data[j][i] = A->data[i][j];
        }
    }
}


/**@brief Multiply two matrices
 *
 * @param A Pointer to the first matrix
 * @param B Pointer to the second matrix
 * @param C Pointer to the result matrix
 */
char transpose_matrix_matrix_mult(struct matrix* A, struct matrix* B, struct matrix* C)
{
    if (A->cols != B->rows)
    {
        fprintf(stderr, "Error: Matrix dimensions do not match\n");
        return -1;
    }
    if (C->rows != A->rows || C->cols != B->cols)
    {
        fprintf(stderr, "Error: Result matrix dimensions do not match\n");
        return -1;
    }

    struct matrix* tmp_b = init_matrix(B->cols, B->rows);
    transpose_matrix(B, tmp_b);

    for (size_t i = 0; i < A->rows; i++)
    {
        for (size_t j = 0; j < B->cols; j++)
        {
            C->data[i][j] = 0;
            for (size_t k = 0; k < A->cols; k++)
            {
                C->data[i][j] += A->data[i][k] * tmp_b->data[j][k];
            }
        }
    }

    free_matrix(tmp_b);
    return 0;
}

/**@brief Add two matrices
 *
 * @param A Pointer to the first matrix
 * @param B Pointer to the second matrix
 * @param C Pointer to the result matrix
 */
char transpose_matrix_matrix_add(struct matrix* A, struct matrix* B, struct matrix* C)
{
    if (A->rows != B->rows || A->cols != B->cols)
    {
        fprintf(stderr, "Error: Matrix dimensions do not match\n");
        return -1;
    }
    if (C->rows != A->rows || C->cols != A->cols)
    {
        fprintf(stderr, "Error: Result matrix dimensions do not match\n");
        return -1;
    }

    for (size_t i = 0; i < A->rows; i++)
    {
        for (size_t j = 0; j < A->cols; j++)
        {
            C->data[i][j] = A->data[i][j] + B->data[i][j];
        }
    }

    return 0;
}

/**@brief Naive GEMM implementation a*A*B + b*C
 *
 * @param alpha Scalar multiplier for A*B
 * @param beta Scalar multiplier for C
 * @param A Pointer to the first matrix
 * @param B Pointer to the second matrix
 * @param C Pointer to the accumulator matrix
 * @param D Pointer to the result matrix
 * @param rows Number of rows
 * @param cols Number of columns
 */
char transpose_gemm(double alpha, double beta, struct matrix* A, struct matrix* B, struct matrix* C, struct matrix* D)
{
    char status = 0;
    if (D == NULL)
    {
        D = init_matrix(A->rows, B->cols);
    }
    status = transpose_matrix_matrix_mult(A, B, D);

    if (status != 0)
    {
        fprintf(stderr, "Error: Matrix multiplication failed\n");
        return -1;
    }

    transpose_matrix_num_mult(alpha, D, D);


    struct matrix* tmp = init_matrix(C->rows, C->cols);
    transpose_matrix_num_mult(beta, C, tmp);

    status = transpose_matrix_matrix_add(D, tmp, D);
    if (status != 0)
    {
        fprintf(stderr, "Error: Matrix addition failed\n");
        return -1;
    }

    return 0;
}

char transpose_gemm_default(struct matrix* A, struct matrix* B, struct matrix* C, struct matrix* D)
{
    return transpose_gemm(1.0, 1.0, A, B, C, D);
}
