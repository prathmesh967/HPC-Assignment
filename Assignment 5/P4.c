#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int N = 1000000;
    int *arr = malloc(N * sizeof(int));
    int *pre = malloc(N * sizeof(int));

    for (int i = 0; i < N; i++)
        arr[i] = 1;

    double st = omp_get_wtime();

    pre[0] = arr[0];
    for (int i = 1; i < N; i++)
        pre[i] = pre[i - 1] + arr[i];

    double end = omp_get_wtime();

    printf("Prefix sum completed in %f seconds\n", end - st);
    printf("Last prefix value: %d\n", pre[N-1]);

    free(arr);
    free(pre);
    return 0;
}
