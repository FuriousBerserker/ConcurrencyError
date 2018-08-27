#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int A[2][4] = {{0, 1, 2, 3}, {0, 0, 0, 0}};

int main() {
    
    //#pragma omp target device(0)
    #pragma omp parallel num_threads(4)
    {
        int threadID = omp_get_thread_num();
        if (threadID == 0) {
            printf("Is on the initial device = %d\n", omp_is_initial_device());
        }
        int buf = 0;
        int x = (threadID == 0 ? 4 : 1);
        int y = (threadID == 0 ? 1 : 4);
        int i = 0;
        int j;
        while (i < x) {
            j = 0;
            while (j < y) {
                #pragma omp barrier
                A[1 - buf][threadID] = A[buf][(threadID + 1) % 4];
                buf = 1 - buf;
                j++;
            }
            i++;
        }
    }

    printf("{{%d, %d, %d, %d}, {%d, %d, %d, %d}}\n", A[0][0], A[0][1], A[0][2], A[0][3], A[1][0], A[1][1], A[1][2], A[1][3]);
    return 0;
}
