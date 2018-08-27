#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10000000
#define TEAM_NUM 100
#define THREAD_LIMIT 1024

int main() {
    long *array, i;
    array = (long*) malloc(sizeof(long) * N);
    for (i = 0; i < N; i++) {
       array[i] = i; 
    }

    #pragma omp target device(0) map(tofrom: array[0: N])
    #pragma omp teams distribute parallel for num_teams(TEAM_NUM) thread_limit(THREAD_LIMIT)
    for (i = 0; i < N; i++) {
        array[i] *= 2;
    }

    #pragma omp target device(0) map(tofrom: array[0: N])
    #pragma omp teams distribute parallel for num_teams(TEAM_NUM) thread_limit(THREAD_LIMIT) schedule(static, 1)
    for (i = 0; i < N; i++) {
        array[i] *= 3;
    }
    
    for (i = 0; i < 10; i++) {
        printf("array[%ld] = %ld\n", i, array[i]);
    }

    return 0;
}
