#include <stdio.h>
#include "GEMM/gemm.h"
#include "GEMM/matrix.h"
#include "GEMM/naive/naive_gemm.h"
#include "GEMM/transpose/transpose_gemm.h"

enum gemm_type gemm_type;

int gemm(enum gemm_type type, double alpha, double beta, struct matrix* A, struct matrix* B, struct matrix* C, struct matrix* D)
{
    switch (type)
    {
        case GEMM_NAIVE:
            return naive_gemm(alpha, beta, A, B, C, D);
        case GEMM_TRANSPOSE:
            return transpose_gemm(alpha, beta, A, B, C, D);
        //case GEMM_WINOGRAD:
        //    return winograd_gemm(alpha, beta, A, B, C, D);
        default:
            fprintf(stderr, "Error: Invalid GEMM type\n");
            return 1;
    }
    return 0;
}

int gemm_default(enum gemm_type type, struct matrix* A, struct matrix* B, struct matrix* C, struct matrix* D)
{
    return gemm(type, 1.0, 1.0, A, B, C, D);
}
