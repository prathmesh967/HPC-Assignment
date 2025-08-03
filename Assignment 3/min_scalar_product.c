#include <stdio.h>
#include <omp.h>

int main() {
    int n;
    printf("Enter size of vectors: ");
    scanf("%d", &n);

    int A[n], B[n];
    for(int i = 0; i < n; i++) {
        A[i] = i+1;     // example values
        B[i] = n-i;     // reverse order
    }

    int min_product = 99999999; // large value

    #pragma omp parallel for reduction(min:min_product)
    for(int i = 0; i < n; i++) {
        int product = A[i] * B[i];
        if(product < min_product)
            min_product = product;
    }

    printf("Minimum Scalar Product = %d\n", min_product);
    return 0;
}

