#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int A = 0;
    volatile int flag = 0;

    #pragma omp target map(tofrom: A, flag)
    #pragma omp teams num_teams(2)
    //#pragma omp parallel num_threads(2) shared(A, flag)
    {
        printf("is on initial device = %d\n", omp_is_initial_device());
        int teamID = omp_get_team_num();
        //int teamID = omp_get_thread_num();
        if (teamID == 1) {
            A = 1;
            flag = 1;
        } else if (teamID == 0) {
            while (flag == 0) {
            //#pragma omp flush(flag)
            }
            printf("A = %d, flag = %d\n", A, flag);
        }
        //printf("A = %d, flag = %d\n", A, flag);
    }
    return 0;
}
