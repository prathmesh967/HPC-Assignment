#include <stdio.h>
#include <omp.h>
#define SIZE 200

void static_sched(int chunk) {
    int a[SIZE], s = 5;
    for (int i = 0; i < SIZE; i++) a[i] = i;

    double t1 = omp_get_wtime();
    #pragma omp parallel for schedule(static, chunk)
    for (int i = 0; i < SIZE; i++) a[i] += s;
    double t2 = omp_get_wtime();

    printf("Static %d: %f\n", chunk, t2 - t1);
}

void dynamic_sched(int chunk) {
    int a[SIZE], s = 5;
    for (int i = 0; i < SIZE; i++) a[i] = i;

    double t1 = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic, chunk)
    for (int i = 0; i < SIZE; i++) a[i] += s;
    double t2 = omp_get_wtime();

    printf("Dynamic %d: %f\n", chunk, t2 - t1);
}

void nowait_demo() {
    int a[SIZE], b[SIZE], s = 10;
    for (int i = 0; i < SIZE; i++) {
        a[i] = i;
        b[i] = i;
    }

    #pragma omp parallel
    {
        #pragma omp for nowait
        for (int i = 0; i < SIZE; i++) a[i] += s;

        #pragma omp for
        for (int i = 0; i < SIZE; i++) b[i] *= 2;
    }

    printf("Nowait done\n");
}

int main() {
    omp_set_num_threads(4);
    static_sched(10);
    dynamic_sched(10);
    nowait_demo();
    return 0;
}
