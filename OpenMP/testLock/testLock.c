#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>

#define TEAM_NUM 4

int main() {
    unsigned count[TEAM_NUM] = {0};
    unsigned total = 0;
    #pragma omp target data device(1) map(tofrom: count[0:TEAM_NUM])
    {
        #pragma omp target device(1) map(tofrom:total)
        #pragma omp teams num_teams(TEAM_NUM) shared(total)
        {
                omp_lock_t lck;
                unsigned team_id = omp_get_team_num();
                omp_init_lock(&lck);
                count[team_id] += 1;
                omp_set_lock(&lck);
                printf("team num = %d, team id = %d, thread_limit = %d, is initial device = %d\n", omp_get_num_teams(), omp_get_team_num(), omp_get_thread_limit(), omp_is_initial_device());
                total += 1;
                omp_unset_lock(&lck);
        }
    }
    for (unsigned i = 0; i < TEAM_NUM; i++) {
        printf("count[%d], expected = %d, actual = %d\n", i, 1, count[i]);
    }
    printf("total, expected = %d, actual = %d\n", TEAM_NUM, total);
    return 0;
}
