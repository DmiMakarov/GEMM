#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>
#include "GEMM/strassen/strassen.h"


//#TODO: Implement Strassen's algorithm
//#TODO: Implement padding for matricies to power of two sizes
//#TODO: Implement reverse padding
//#TODO: Implement recursive matrix multiplication

static size_t max_of_3(size_t a, size_t b, size_t c)
{
    return (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
}

static size_t next_pow2(size_t n)
{
    if (n <= 1) return 1;

    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
#if SIZE_MAX > UINT32_MAX
    n |= n >> 32;
#endif

    return n + 1;
}

int matrix_padding(struct matrix* A, struct matrix* Out)
{
    for (size_t i = 0; i < A->rows; i++)
    {
        for (size_t j = 0; j < A->cols; j++)
        {
            Out->data[i][j] = A->data[i][j];
        }
    }

    for (size_t i = A->rows; i < Out->rows; i++)
    {
        for (size_t j = A->cols; j < Out->cols; j++)
        {
            Out->data[i][j] = 0;
        }
    }

    return 0;
}

int matrix_reverse_padding(struct matrix* A, struct matrix* Out)
{
    for (size_t i = 0; i < A->rows; i++)
    {
        for (size_t j = 0; j < A->cols; j++)
        {
            Out->data[i][j] = A->data[i][j];
        }
    }

    return 0;
}

int strassen_matrix_matrix_mult_recursive(struct matrix* A, struct matrix* B, struct matrix* C)


int strassen_matrix_matrix_mult(struct matrix* A, struct matrix* B, struct matrix* C)
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

    size_t padded_size = next_pow2(max_of_3(A->rows, A->cols, B->cols));
    struct matrix* A_padded = init_matrix(padded_size, padded_size);
    struct matrix* B_padded = init_matrix(padded_size, padded_size);
    struct matrix* C_padded = init_matrix(padded_size, padded_size);

    matrix_padding(A, A_padded);
    matrix_padding(B, B_padded);
    matrix_padding(C, C_padded);

    matrix_reverse_padding(C_padded, C);

    free_matrix(A_padded);
    free_matrix(B_padded);
    free_matrix(C_padded);

    return 0;
}

/**@brief Add two matrices
 *
 * @param A Pointer to the first matrix
 * @param B Pointer to the second matrix
 * @param C Pointer to the result matrix
 */
int strassen_matrix_matrix_add(struct matrix* A, struct matrix* B, struct matrix* C)
{
    if (A->rows != B->rows || A->cols != B->cols)
    {
        fprintf(stderr, "Error: Matrix dimensions do not match\n");
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


/**@brief Multiply a matrix by a scalar
 *
 * @param alpha Scalar multiplier
 * @param A Pointer to the matrix
 * @param B Pointer to the result matrix
 */
int strassen_matrix_num_mult(double alpha, struct matrix* A, struct matrix* B)
{
    if (A->rows != B->rows || A->cols != B->cols)
    {
        fprintf(stderr, "Error: Matrix dimensions do not match\n");
        return -1;
    }

    for (size_t i = 0; i < A->rows; i++)
    {
        for (size_t j = 0; j < A->cols; j++)
        {
            B->data[i][j] = alpha * A->data[i][j];
        }
    }
    return 0;
}

/**@brief Strassen GEMM implementation a*A*B + b*C
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
int strassen_gemm(double alpha, double beta, struct matrix* A, struct matrix* B, struct matrix* C, struct matrix* D)
{
    int status = 0;

    if (D == NULL)
    {
        fprintf(stderr, "Error: Result matrix is NULL\n");
        return -1;
    }
    status = strassen_matrix_matrix_mult(A, B, D);

    if (status != 0)
    {
        fprintf(stderr, "Error: Matrix multiplication failed\n");
        return -1;
    }

    strassen_matrix_num_mult(alpha, D, D);

    if (C != NULL)
    {
        struct matrix* tmp = init_matrix(C->rows, C->cols);
        if (tmp == NULL) {
            fprintf(stderr, "Error: Failed to allocate memory for temporary matrix\n");
            return -1;
        }
        strassen_matrix_num_mult(beta, C, tmp);
        status = strassen_matrix_matrix_add(D, tmp, D);
        if (status != 0)
        {
            fprintf(stderr, "Error: Matrix addition failed\n");
            free_matrix(tmp);
            return -1;
        }
        free_matrix(tmp);
    }

    return 0;
}
