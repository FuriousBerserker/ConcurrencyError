#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define N 1000000
#define TEAM_NUM 100
#define THREAD_LIMIT 1024

int main() {
    
    unsigned long long int atomic_x = 0;
    unsigned x = 0;
    unsigned i = 0;
    #pragma omp target device(0) map(tofrom: atomic_x, x)
    #pragma omp teams distribute parallel for num_teams(TEAM_NUM) thread_limit(THREAD_LIMIT)
    for (i = 0; i < N; i++) {
        x++;
        #pragma omp atomic update
        atomic_x++;
    }

    printf("x, expected = %d, actual = %d\n", N, x);
    printf("atomic_x expected = %d, actual = %lld\n", N, atomic_x);
    
    return 0;
}
