#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"
#include "strassen.h"


static void test_strassen_matrix_num_mult()
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

    strassen_matrix_num_mult(2.0, A, B);
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

static void test_strassen_matrix_matrix_mult()
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

    strassen_matrix_matrix_mult(A, B, C);
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

static void test_strassen_matrix_matrix_add()
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

    strassen_matrix_matrix_add(A, B, C);
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

static void test_strassen_gemm()
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

    strassen_gemm(1.0, 1.0, A, B, C, D);
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

    assert(strassen_gemm(1.0, 2.0, A, B, C, D) == -1);
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
    free_matrix(D);
}

static void test_strassen_non_square()
{
    // 2x3 * 3x2 = 2x2 -- tests padding to power-of-2
    struct matrix* A = init_matrix(2, 3);
    struct matrix* B = init_matrix(3, 2);
    struct matrix* C = init_matrix(2, 2);

    A->data[0][0] = 1.0; A->data[0][1] = 2.0; A->data[0][2] = 3.0;
    A->data[1][0] = 4.0; A->data[1][1] = 5.0; A->data[1][2] = 6.0;

    B->data[0][0] = 7.0;  B->data[0][1] = 8.0;
    B->data[1][0] = 9.0;  B->data[1][1] = 10.0;
    B->data[2][0] = 11.0; B->data[2][1] = 12.0;

    strassen_matrix_matrix_mult(A, B, C);
    // Row 0: 1*7+2*9+3*11=58, 1*8+2*10+3*12=64
    // Row 1: 4*7+5*9+6*11=139, 4*8+5*10+6*12=154
    assert(C->data[0][0] == 58.0);
    assert(C->data[0][1] == 64.0);
    assert(C->data[1][0] == 139.0);
    assert(C->data[1][1] == 154.0);
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
}

static void test_strassen_identity()
{
    // A * I = A
    struct matrix* A = init_matrix(4, 4);
    struct matrix* I = init_matrix(4, 4);
    struct matrix* C = init_matrix(4, 4);

    for (size_t i = 0; i < 4; i++)
        for (size_t j = 0; j < 4; j++)
        {
            A->data[i][j] = (double)(i * 4 + j + 1);
            I->data[i][j] = (i == j) ? 1.0 : 0.0;
        }

    strassen_matrix_matrix_mult(A, I, C);
    for (size_t i = 0; i < 4; i++)
        for (size_t j = 0; j < 4; j++)
            assert(C->data[i][j] == A->data[i][j]);

    free_matrix(A);
    free_matrix(I);
    free_matrix(C);
}

int main()
{
    test_strassen_matrix_num_mult();
    test_strassen_matrix_matrix_mult();
    test_strassen_matrix_matrix_add();
    test_strassen_gemm();
    test_wrong_dimensions();
    test_strassen_non_square();
    test_strassen_identity();
    return 0;
}
