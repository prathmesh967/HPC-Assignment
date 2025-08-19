#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int r = 500, c = 500, scalar = 5;
    int A[r][c];
    int i, j;

    for (i = 0; i < r; i++)
        for (j = 0; j < c; j++)
            A[i][j] = rand() % 10;

    double start = omp_get_wtime();
    #pragma omp parallel for private(i, j) shared(A, scalar, r, c)
    for (i = 0; i < r; i++)
        for (j = 0; j < c; j++)
            A[i][j] *= scalar;
    double end = omp_get_wtime();

    printf("Matrix-Scalar Multiplication Time: %f\n", end - start);
    return 0;
}
