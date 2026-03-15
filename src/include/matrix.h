#ifndef MATRIX_H
#define MATRIX_H
/**
 * @brief Matrix structure
 *
 * @param rows Number of rows
 * @param cols Number of columns
 * @param data Pointer to the data
 */

#include <stddef.h>


struct matrix {
    size_t rows;
    size_t cols;

    double **data;
};

/**
 * @brief Initialize a matrix
 *
 * @param rows Number of rows
 * @param cols Number of columns
 * @return Pointer to the matrix
 */
struct matrix* init_matrix(size_t rows, size_t cols);

/**
 * @brief Free a matrix
 *
 * @param mat Pointer to the matrix
 */
void free_matrix(struct matrix* mat);

/**
 * @brief Print a matrix
 *
 * @param mat Pointer to the matrix
 */
void print_matrix(struct matrix* mat);

#endif // MATRIX_H
