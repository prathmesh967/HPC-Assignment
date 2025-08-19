#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int r = 500, c = 500;
    int A[r][c], x[c], y[r];
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            A[i][j] = rand() % 10;
    for (int i = 0; i < c; i++)
        x[i] = rand() % 10;

    double start = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < r; i++) {
        y[i] = 0;
        for (int j = 0; j < c; j++)
            y[i] += A[i][j] * x[j];
    }
    double end = omp_get_wtime();

    printf("Matrix-Vector Multiplication Time: %f\n", end - start);
    return 0;
}
