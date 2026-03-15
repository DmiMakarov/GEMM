#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"
#include "transpose_gemm.h"


static void test_transpose_matrix_num_mult()
{
    struct matrix* A = init_matrix(3, 3);
    struct matrix* B = init_matrix(3, 3);

    A->data[0][0] = 1.0;
    A->data[0][1] = 2.0;
    A->data[0][2] = 3.0;
    A->data[1][0] = 4.0;
    A->data[1][1] = 5.0;
    A->data[1][2] = 6.0;
    A->data[2][0] = 7.0;
    A->data[2][1] = 8.0;
    A->data[2][2] = 9.0;

    B->data[0][0] = 1.0;
    B->data[0][1] = 2.0;
    B->data[0][2] = 3.0;
    B->data[1][0] = 4.0;
    B->data[1][1] = 5.0;
    B->data[1][2] = 6.0;
    B->data[2][0] = 7.0;
    B->data[2][1] = 8.0;
    B->data[2][2] = 9.0;

    transpose_matrix_num_mult(2.0, A, B);
    assert(B->data[0][0] == 2.0);
    assert(B->data[0][1] == 4.0);
    assert(B->data[0][2] == 6.0);
    assert(B->data[1][0] == 8.0);
    assert(B->data[1][1] == 10.0);
    assert(B->data[1][2] == 12.0);
    assert(B->data[2][0] == 14.0);
    assert(B->data[2][1] == 16.0);
    assert(B->data[2][2] == 18.0);
    free_matrix(A);
    free_matrix(B);
}

static void test_transpose_matrix_matrix_mult()
{
    struct matrix* A = init_matrix(3, 3);
    struct matrix* B = init_matrix(3, 3);
    struct matrix* C = init_matrix(3, 3);

    A->data[0][0] = 1.0;
    A->data[0][1] = 2.0;
    A->data[0][2] = 3.0;
    A->data[1][0] = 4.0;
    A->data[1][1] = 5.0;
    A->data[1][2] = 6.0;
    A->data[2][0] = 7.0;
    A->data[2][1] = 8.0;
    A->data[2][2] = 9.0;

    B->data[0][0] = 1.0;
    B->data[0][1] = 2.0;
    B->data[0][2] = 3.0;
    B->data[1][0] = 4.0;
    B->data[1][1] = 5.0;
    B->data[1][2] = 6.0;
    B->data[2][0] = 7.0;
    B->data[2][1] = 8.0;
    B->data[2][2] = 9.0;

    transpose_matrix_matrix_mult(A, B, C);
    assert(C->data[0][0] == 30.0);
    assert(C->data[0][1] == 36.0);
    assert(C->data[0][2] == 42.0);
    assert(C->data[1][0] == 66.0);
    assert(C->data[1][1] == 81.0);
    assert(C->data[1][2] == 96.0);
    assert(C->data[2][0] == 102.0);
    assert(C->data[2][1] == 126.0);
    assert(C->data[2][2] == 150.0);
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
}

static void test_transpose_matrix_matrix_add()
{
    struct matrix* A = init_matrix(3, 3);
    struct matrix* B = init_matrix(3, 3);
    struct matrix* C = init_matrix(3, 3);

    A->data[0][0] = 1.0;
    A->data[0][1] = 2.0;
    A->data[0][2] = 3.0;
    A->data[1][0] = 4.0;
    A->data[1][1] = 5.0;
    A->data[1][2] = 6.0;
    A->data[2][0] = 7.0;
    A->data[2][1] = 8.0;
    A->data[2][2] = 9.0;

    B->data[0][0] = 1.0;
    B->data[0][1] = 2.0;
    B->data[0][2] = 3.0;
    B->data[1][0] = 4.0;
    B->data[1][1] = 5.0;
    B->data[1][2] = 6.0;
    B->data[2][0] = 7.0;
    B->data[2][1] = 8.0;
    B->data[2][2] = 9.0;

    transpose_matrix_matrix_add(A, B, C);
    assert(C->data[0][0] == 2.0);
    assert(C->data[0][1] == 4.0);
    assert(C->data[0][2] == 6.0);
    assert(C->data[1][0] == 8.0);
    assert(C->data[1][1] == 10.0);
    assert(C->data[1][2] == 12.0);
    assert(C->data[2][0] == 14.0);
    assert(C->data[2][1] == 16.0);
    assert(C->data[2][2] == 18.0);
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
}

static void test_transpose_gemm()
{
    struct matrix* A = init_matrix(3, 3);
    struct matrix* B = init_matrix(3, 3);
    struct matrix* C = init_matrix(3, 3);
    struct matrix* D = init_matrix(3, 3);

    A->data[0][0] = 1.0;
    A->data[0][1] = 2.0;
    A->data[0][2] = 3.0;
    A->data[1][0] = 4.0;
    A->data[1][1] = 5.0;
    A->data[1][2] = 6.0;
    A->data[2][0] = 7.0;
    A->data[2][1] = 8.0;
    A->data[2][2] = 9.0;

    B->data[0][0] = 1.0;
    B->data[0][1] = 2.0;
    B->data[0][2] = 3.0;
    B->data[1][0] = 4.0;
    B->data[1][1] = 5.0;
    B->data[1][2] = 6.0;
    B->data[2][0] = 7.0;
    B->data[2][1] = 8.0;
    B->data[2][2] = 9.0;

    C->data[0][0] = 1.0;
    C->data[0][1] = 2.0;
    C->data[0][2] = 3.0;
    C->data[1][0] = 4.0;
    C->data[1][1] = 5.0;
    C->data[1][2] = 6.0;
    C->data[2][0] = 7.0;
    C->data[2][1] = 8.0;
    C->data[2][2] = 9.0;

    transpose_gemm(1.0, 1.0, A, B, C, D);
    assert(D->data[0][0] == 31.0);
    assert(D->data[0][1] == 38.0);
    assert(D->data[0][2] == 45.0);
    assert(D->data[1][0] == 70.0);
    assert(D->data[1][1] == 86.0);
    assert(D->data[1][2] == 102.0);
    assert(D->data[2][0] == 109.0);
    assert(D->data[2][1] == 134.0);
    assert(D->data[2][2] == 159.0);
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
    free_matrix(D);
}

static void test_wrong_dimensions()
{
    struct matrix* A = init_matrix(3, 3);
    struct matrix* B = init_matrix(2, 2);
    struct matrix* C = init_matrix(3, 3);
    struct matrix* D = init_matrix(3, 3);

    assert(transpose_gemm(1.0, 2.0, A, B, C, D) == -1);
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
    free_matrix(D);
}

int main()
{
    test_transpose_matrix_num_mult();
    test_transpose_matrix_matrix_mult();
    test_transpose_matrix_matrix_add();
    test_transpose_gemm();
    test_wrong_dimensions();
    return 0;
}
