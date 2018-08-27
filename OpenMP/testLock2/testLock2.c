
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <omp.h>

#define TEAM_NUM 100
#define THREAD_LIMIT 512

int main() {
    #pragma omp target device(1)
    #pragma omp teams num_teams(TEAM_NUM) thread_limit(THREAD_LIMIT)
    {

        omp_lock_t lck;
        omp_init_lock(&lck);
        unsigned aliveThread = 0;
        #pragma omp distribute parallel for schedule(static,1)
        for (unsigned i = 0; i < THREAD_LIMIT; i++) {
                while (1) {
                omp_set_lock(&lck);
                //#pragma omp flush(aliveThread)
                if (aliveThread != 0) {
                    printf("alive thread = %d\n", aliveThread);
                }
                aliveThread += 1;
                //printf("team num = %d, team id = %d, thread_limit = %d, is initial device = %d, alive thread = %d\n", omp_get_num_teams(), omp_get_team_num(), omp_get_thread_limit(), omp_is_initial_device(), aliveThread);
                aliveThread -= 1;
                omp_unset_lock(&lck);
                }
        }
    }
    return 0;
}
