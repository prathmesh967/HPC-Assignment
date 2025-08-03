#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main() {
    int n, scalar;
    printf("Enter size of vector: ");
    scanf("%d", &n);
    printf("Enter scalar value: ");
    scanf("%d", &scalar);

    int *vector = (int*) malloc(n * sizeof(int));

    // Initialize vector
    for(int i = 0; i < n; i++) {
        vector[i] = i;
    }

    double start_time = omp_get_wtime();

    // Parallel scalar addition
    #pragma omp parallel for
    for(int i = 0; i < n; i++) {
        vector[i] += scalar;
    }

    double end_time = omp_get_wtime();

    printf("Vector after scalar addition:\n");
    for(int i = 0; i < (n > 10 ? 10 : n); i++) {
        printf("%d ", vector[i]);
    }
    printf("\nExecution time: %lf seconds\n", end_time - start_time);

    free(vector);
    return 0;
}
