#include <stdio.h>
#include <stdlib.h>
#include "GEMM/matrix.h"


/**
 * @brief Initialize a matrix
 *
 * @param rows Number of rows
 * @param cols Number of columns
 * @return Pointer to the matrix
 */
struct matrix* init_matrix(size_t rows, size_t cols) {
    struct matrix* mat = (struct matrix*)malloc(sizeof(struct matrix));

    if (mat == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for matrix\n");
        return NULL;
    }

    mat->rows = rows;
    mat->cols = cols;
    mat->data = (double**)malloc(rows * sizeof(double*));

    if (mat->data == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for matrix data\n");
        free(mat);
        return NULL;
    }

    for (size_t i = 0; i < rows; i++) {
        mat->data[i] = (double*)malloc(cols * sizeof(double));
        if (mat->data[i] == NULL) {
            fprintf(stderr, "Error: Failed to allocate memory for matrix data row\n");
            free(mat->data);
            free(mat);
            return NULL;
        }
    }

    return mat;
}

/**
 * @brief Free a matrix
 *
 * @param mat Pointer to the matrix
 */
void free_matrix(struct matrix* mat) {

    if (mat == NULL) {
        fprintf(stderr, "Error: Matrix is NULL\n");
        return;
    }

    for (size_t i = 0; i < mat->rows; i++)
    {
        free(mat->data[i]);
    }
    free(mat->data);
    free(mat);
}

/**
 * @brief Print a matrix
 *
 * @param mat Pointer to the matrix
 */
void print_matrix(struct matrix* mat) {
    if (mat == NULL) {
        fprintf(stderr, "Error: Matrix is NULL\n");
        return;
    }

    for (size_t i = 0; i < mat->rows; i++) {
        for (size_t j = 0; j < mat->cols; j++) {
            printf("%f\t", mat->data[i][j]);
        }
        printf("\n");
    }
}
