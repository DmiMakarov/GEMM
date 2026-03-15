#include <assert.h>
#include <stdlib.h>
#include "matrix.h"


#define EPSILON 1e-6

static void test_init_matrix()
{
    struct matrix* mat = init_matrix(3, 3);
    assert(mat != NULL);
    assert(mat->rows == 3);
    assert(mat->cols == 3);
    free_matrix(mat);
}

static void test_free_matrix()
{
    struct matrix* mat = init_matrix(3, 3);
    free_matrix(mat);
}

static void test_print_matrix()
{
    struct matrix* mat = init_matrix(3, 3);
    print_matrix(mat);
    free_matrix(mat);
}

int main()
{
    test_init_matrix();
    test_free_matrix();
    test_print_matrix();
    return 0;
}
