#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 500 

int main() {
    static int A[N][N], B[N][N], C[N][N];
    int i, j, k;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++) 
        {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
            C[i][j] = 0;
        }
    }

    double st = omp_get_wtime();

    #pragma omp parallel for private(i, j, k) shared(A, B, C)
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
             for (k = 0; k < N; k++)
             {
                 C[i][j] += A[i][k] * B[k][j];
             }
        }
    }
    double end = omp_get_wtime();
    printf("Matrix-Matrix Multiplication completed in %f seconds\n", end - st);

    return 0;
}
