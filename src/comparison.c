#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "GEMM/matrix.h"
#include "GEMM/gemm.h"

void generate_random_matrix(struct matrix* mat)
{
    for (size_t i = 0; i < mat->rows; i++)
    {
        for (size_t j = 0; j < mat->cols; j++)
        {
            mat->data[i][j] = (double)rand() / RAND_MAX;
        }
    }
}


double measure_time(enum gemm_type type, struct matrix* A, struct matrix* B, struct matrix* C, struct matrix* D)
{
    struct timespec start, end;
    timespec_get(&start, TIME_UTC);
    gemm_default(type, A, B, C, D);
    timespec_get(&end, TIME_UTC);

    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

void comparison(size_t num_cycles, size_t a_rows, size_t a_cols, size_t b_rows, size_t b_cols)
{
    struct matrix* A = init_matrix(a_rows, a_cols);
    struct matrix* B = init_matrix(b_rows, b_cols);
    struct matrix* C = init_matrix(a_rows, b_cols);
    struct matrix* D = init_matrix(a_rows, b_cols);

    if (A == NULL || B == NULL || C == NULL || D == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory for matrices\n");
        return;
    }

    generate_random_matrix(A);
    generate_random_matrix(B);
    generate_random_matrix(C);
    generate_random_matrix(D);

    double naive_time[num_cycles];
    double transpose_time[num_cycles];

    for (size_t i = 0; i < num_cycles; i++)
    {
        printf("Cycle %zu\n", i);
        naive_time[i] = measure_time(GEMM_NAIVE, A, B, C, D);
        printf("\tNaive time: %f seconds\n", naive_time[i]);
        transpose_time[i] = measure_time(GEMM_TRANSPOSE, A, B, C, D);
        printf("\tTranspose time: %f seconds\n", transpose_time[i]);
    }

    double naive_time_avg = 0.0;
    double transpose_time_avg = 0.0;
    for (size_t i = 0; i < num_cycles; i++)
    {
        naive_time_avg += naive_time[i];
        transpose_time_avg += transpose_time[i];
    }

    naive_time_avg /= num_cycles;
    transpose_time_avg /= num_cycles;

    double naive_time_std = 0.0;
    double transpose_time_std = 0.0;
    for (size_t i = 0; i < num_cycles; i++)
    {
        naive_time_std += (naive_time[i] - naive_time_avg) * (naive_time[i] - naive_time_avg);
        transpose_time_std += (transpose_time[i] - transpose_time_avg) * (transpose_time[i] - transpose_time_avg);
    }
    naive_time_std = sqrt(naive_time_std / num_cycles);
    transpose_time_std = sqrt(transpose_time_std / num_cycles);

    printf("Naive time: %f seconds (average) ± %f seconds (standard deviation)\n", naive_time_avg, naive_time_std);
    printf("Transpose time: %f seconds (average) ± %f seconds (standard deviation)\n", transpose_time_avg, transpose_time_std);

    free_matrix(A);
    free_matrix(B);
    free_matrix(C);
    free_matrix(D);
}


int main()
{
    comparison(10, 1000, 1000, 1000, 1000);
    return 0;
}
