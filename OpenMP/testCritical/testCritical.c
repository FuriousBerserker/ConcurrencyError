#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define N 1000000
#define TEAM_NUM 512
#define THREAD_LIMIT 512

int main() {
    unsigned *a;
    unsigned num1[1] = {0};
    unsigned i;
    unsigned num2 = 0;
    a = (unsigned*)malloc(sizeof(unsigned) * N);
    #pragma omp target data device(0) map(tofrom: a[0:N]) map(tofrom: num1[0:1], num2)
    {
        #pragma omp target map(tofrom: num2)
        #pragma omp teams distribute parallel for num_teams(TEAM_NUM) thread_limit(THREAD_LIMIT) 
        for (i = 0; i < N; i++) {
            a[i] = 2 * i;
            #pragma omp critical(la)
            {
                num1[0] = num1[0] + 1;
                num2 = num2 + 1;
            }
        }
    }
    for (i = 0; i < N; i++) {
        //printf("%d\n", a[i]);
        if (a[i] != i * 2) {
            assert(0);
        }
    }
    printf("expect num1 = %d, actual value = %d\n", N, num1[0]);
    printf("expect num2 = %d, actual value = %d\n", N, num2);
    free(a);
    return 0;
}
