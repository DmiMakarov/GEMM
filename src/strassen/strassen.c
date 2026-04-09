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
        for (size_t j = A->cols; j < Out->cols; j++)
        {
            Out->data[i][j] = 0;
        }
    }

    for (size_t i = A->rows; i < Out->rows; i++)
    {
        for (size_t j = 0; j < Out->cols; j++)
        {
            Out->data[i][j] = 0;
        }
    }

    return 0;
}

int matrix_reverse_padding(struct matrix* A, struct matrix* Out)
{
    for (size_t i = 0; i < Out->rows; i++)
    {
        for (size_t j = 0; j < Out->cols; j++)
        {
            Out->data[i][j] = A->data[i][j];
        }
    }

    return 0;
}

static void submatrix_extract(struct matrix* M, size_t row_off, size_t col_off, struct matrix* out)
{
    for (size_t i = 0; i < out->rows; i++)
    {
        for (size_t j = 0; j < out->cols; j++)
        {
            out->data[i][j] = M->data[row_off + i][col_off + j];
        }
    }
}

static void submatrix_insert(struct matrix* sub, struct matrix* M, size_t row_off, size_t col_off)
{
    for (size_t i = 0; i < sub->rows; i++)
    {
        for (size_t j = 0; j < sub->cols; j++)
        {
            M->data[row_off + i][col_off + j] = sub->data[i][j];
        }
    }
}

static void matrix_add(struct matrix* A, struct matrix* B, struct matrix* out)
{
    for (size_t i = 0; i < A->rows; i++)
    {
        for (size_t j = 0; j < A->cols; j++)
        {
            out->data[i][j] = A->data[i][j] + B->data[i][j];
        }
    }
}

static void matrix_sub(struct matrix* A, struct matrix* B, struct matrix* out)
{
    for (size_t i = 0; i < A->rows; i++)
    {
        for (size_t j = 0; j < A->cols; j++)
        {
            out->data[i][j] = A->data[i][j] - B->data[i][j];
        }
    }
}

static void transpose_mult(struct matrix* A, struct matrix* B, struct matrix* C)
{
    size_t n = A->rows;

    // Transpose B in-place (B is always square in the recursive path)
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = i + 1; j < n; j++)
        {
            double tmp = B->data[i][j];
            B->data[i][j] = B->data[j][i];
            B->data[j][i] = tmp;
        }
    }

    // Multiply A * B^T — both row accesses are sequential
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = 0; j < n; j++)
        {
            double sum = 0.0;
            for (size_t k = 0; k < n; k++)
            {
                sum += A->data[i][k] * B->data[j][k];
            }
            C->data[i][j] = sum;
        }
    }

    // Restore B
    for (size_t i = 0; i < n; i++)
    {
        for (size_t j = i + 1; j < n; j++)
        {
            double tmp = B->data[i][j];
            B->data[i][j] = B->data[j][i];
            B->data[j][i] = tmp;
        }
    }
}

#define STRASSEN_THRESHOLD 128

int strassen_matrix_matrix_mult_recursive(struct matrix* A, struct matrix* B, struct matrix* C)
{
    size_t n = A->rows;

    if (n <= STRASSEN_THRESHOLD)
    {
        transpose_mult(A, B, C);
        return 0;
    }

    size_t half = n / 2;

    // Allocate all temporaries upfront
    struct matrix* A11 = init_matrix(half, half);
    struct matrix* A12 = init_matrix(half, half);
    struct matrix* A21 = init_matrix(half, half);
    struct matrix* A22 = init_matrix(half, half);
    struct matrix* B11 = init_matrix(half, half);
    struct matrix* B12 = init_matrix(half, half);
    struct matrix* B21 = init_matrix(half, half);
    struct matrix* B22 = init_matrix(half, half);
    struct matrix* M1  = init_matrix(half, half);
    struct matrix* M2  = init_matrix(half, half);
    struct matrix* M3  = init_matrix(half, half);
    struct matrix* M4  = init_matrix(half, half);
    struct matrix* M5  = init_matrix(half, half);
    struct matrix* M6  = init_matrix(half, half);
    struct matrix* M7  = init_matrix(half, half);
    // Two scratch buffers reused for intermediate add/sub results
    struct matrix* tmp1 = init_matrix(half, half);
    struct matrix* tmp2 = init_matrix(half, half);

    if (!A11 || !A12 || !A21 || !A22 ||
        !B11 || !B12 || !B21 || !B22 ||
        !M1 || !M2 || !M3 || !M4 || !M5 || !M6 || !M7 ||
        !tmp1 || !tmp2)
    {
        fprintf(stderr, "Error: Failed to allocate memory in Strassen recursion\n");
        free_matrix(A11); free_matrix(A12); free_matrix(A21); free_matrix(A22);
        free_matrix(B11); free_matrix(B12); free_matrix(B21); free_matrix(B22);
        free_matrix(M1); free_matrix(M2); free_matrix(M3); free_matrix(M4);
        free_matrix(M5); free_matrix(M6); free_matrix(M7);
        free_matrix(tmp1); free_matrix(tmp2);
        return -1;
    }

    // Extract quadrants
    submatrix_extract(A, 0, 0, A11);
    submatrix_extract(A, 0, half, A12);
    submatrix_extract(A, half, 0, A21);
    submatrix_extract(A, half, half, A22);

    submatrix_extract(B, 0, 0, B11);
    submatrix_extract(B, 0, half, B12);
    submatrix_extract(B, half, 0, B21);
    submatrix_extract(B, half, half, B22);

    // M1 = A11 * B11
    strassen_matrix_matrix_mult_recursive(A11, B11, M1);

    // M2 = A12 * B21
    strassen_matrix_matrix_mult_recursive(A12, B21, M2);

    // M3 = A21 * (B22 - B11)    [t4 = B22 - B11]
    matrix_sub(B22, B11, tmp1);
    strassen_matrix_matrix_mult_recursive(A21, tmp1, M3);

    // M4 = A22 * B22
    strassen_matrix_matrix_mult_recursive(A22, B22, M4);

    // M5 = (A21 + A22) * (B21 + B22)    [t1 * t5]
    matrix_add(A21, A22, tmp1);
    matrix_add(B21, B22, tmp2);
    strassen_matrix_matrix_mult_recursive(tmp1, tmp2, M5);

    // M6 = (A22 - A12) * (B22 - B12)    [t2 * t6]
    matrix_sub(A22, A12, tmp1);
    matrix_sub(B22, B12, tmp2);
    strassen_matrix_matrix_mult_recursive(tmp1, tmp2, M6);

    // M7 = (A22 - A11) * B12    [t3 * B12]
    matrix_sub(A22, A11, tmp1);
    strassen_matrix_matrix_mult_recursive(tmp1, B12, M7);

    // Assemble C quadrants using tmp1, tmp2 as scratch
    // C11 = M1 + M2
    matrix_add(M1, M2, tmp1);
    submatrix_insert(tmp1, C, 0, 0);

    // C22 = M5 + M6 - M2 - M4
    matrix_add(M5, M6, tmp1);
    matrix_sub(tmp1, M2, tmp2);
    matrix_sub(tmp2, M4, tmp1);
    submatrix_insert(tmp1, C, half, half);
    // tmp1 now holds C22, keep it for post-corrections

    // C12 = (M5 - M7) - C22
    matrix_sub(M5, M7, tmp2);
    matrix_sub(tmp2, tmp1, tmp2);
    submatrix_insert(tmp2, C, 0, half);

    // C21 = C22 - (M3 + M6)
    matrix_add(M3, M6, tmp2);
    matrix_sub(tmp1, tmp2, tmp2);
    submatrix_insert(tmp2, C, half, 0);

    // Free everything
    free_matrix(A11); free_matrix(A12); free_matrix(A21); free_matrix(A22);
    free_matrix(B11); free_matrix(B12); free_matrix(B21); free_matrix(B22);
    free_matrix(M1); free_matrix(M2); free_matrix(M3); free_matrix(M4);
    free_matrix(M5); free_matrix(M6); free_matrix(M7);
    free_matrix(tmp1); free_matrix(tmp2);

    return 0;
}

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

    strassen_matrix_matrix_mult_recursive(A_padded, B_padded, C_padded);

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
