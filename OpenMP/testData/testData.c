#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>
#define N 100000000

int main() {
    int i;
    float *p, *v1, *v2;

    p = (float*)malloc(sizeof(float) * N);
    v1 = (float*)malloc(sizeof(float) * N);
    v2 = (float*)malloc(sizeof(float) * N);
    
    printf("Device num = %d\n", omp_get_num_devices());

    for (i = 0; i < N; i++) {
       p[i] = 0.0;
       v1[i] = 2.3;
       v2[i] = 10.0;
    }

    #pragma omp target data device(1) map(to: v1[0:N], v2[0:N]) map(from: p[0:N])
    {
        #pragma omp target teams distribute parallel for device(1)
        for (i = 0; i < N; i++) {
            if (i % 10000000 == 3) {
                printf("team num = %d, thread_limit = %d, is on initial device = %d\n", omp_get_num_teams(), omp_get_thread_limit(), omp_is_initial_device());
            }
            p[i] = v1[i] * v2[i];
        }
    }
    
    for (i = 0; i < N; i++) {
        if (p[i] != 23.0) {
            assert(0);
        }
    }
    printf("Completed!\n");
    return 0;
}
