#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>

#define N 2000

int main() {
    
    int* array;
    array = (int*)malloc(sizeof(int) * N);
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        array[i] = i;
    }

    //#pragma omp target device(0) map(tofrom: array[0:N])
    #pragma omp parallel num_threads(N)
    {
        int thread_id = omp_get_thread_num();
        if (thread_id == 0) {
            printf("team num = %d, thread limit = %d, is on the initial device = %d\n", omp_get_num_teams(), omp_get_thread_limit(), omp_is_initial_device());
        }
        if (thread_id % 2 == 0) {
            array[thread_id] = array[thread_id + 1];
            printf("thread %d reaches barrier\n", thread_id);
            #pragma omp barrier
            printf("thread %d leaves barrier\n", thread_id);
        }
    }

    for (int i = 0; i < N; i++) {
        if (i % 2 == 0) {
            if(array[i] != array[i + 1]) {
                printf("Mismatch, array[%d] = %d, array[%d] = %d\n", i, array[i], i + 1, array[i + 1]);
            }
        }
        if (i < 100) {
            printf("array[%d] = %d\n", i, array[i]);
        }
    }
    return 0;
}
