#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 100
#define TEAM_NUM 1
#define THREAD_LIMIT 32

int main() {
    unsigned *array, i;
    array = (unsigned*)malloc(sizeof(unsigned) * N);
    
    omp_set_num_threads(THREAD_LIMIT);
    
    #pragma omp parallel for
    for (i = 0; i < N; i++) {
        array[i] = i;
    }
    
    #pragma omp target device(1) map(tofrom: array[0:N])
    #pragma omp teams distribute parallel for \
    num_teams(TEAM_NUM) \
    thread_limit(THREAD_LIMIT) \
    schedule(static, 1)
    for (i = 0; i < N; i++) {
        array[i] = array[(i + 1) % N];
        if (i == 0) {
            printf("team num = %d, thread_limit = %d, is initial device = %d\n", omp_get_num_teams(), omp_get_thread_limit(), omp_is_initial_device());
        }
    }

    printf("device:\n");
    for (i = 0; i < N; i++) {
        if (array[i] != (i + 1) % N) {
            printf("array[%d] mismatch, expected = %d, actual = %d\n", i, (i + 1) % N, array[i]);
        }
    }

    #pragma omp parallel for
    for (i = 0; i < N; i++) {
        array[i] = i;
    }
    
    #pragma omp parallel for schedule(static, 1)
    for (i = 0; i < N; i++) {
        array[i] = array[(i + 1) % N];
        if (i == 0) {
            printf("thread_num = %d, is initial device = %d\n", omp_get_num_threads(), omp_is_initial_device());
        }
    }

    printf("host:\n");
    for (i = 0; i < N; i++) {
        if (array[i] != (i + 1) % N) {
            printf("array[%d] mismatch, expected = %d, actual = %d\n", i, (i + 1) % N, array[i]);
        }
    }
    return 0;
}
