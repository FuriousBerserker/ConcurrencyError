#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define DISTANCE 128

int main() {
    unsigned long *array;
    array = (unsigned long*)malloc(sizeof(long) * DISTANCE);
    
    for (unsigned i = 0; i < DISTANCE; i++) {
        array[i] = 0l;
    }

    #pragma omp target map(tofrom: array[0:DISTANCE])
    #pragma omp teams num_teams(2)
    {
        int teamID = omp_get_team_num();
        if (teamID == 0) {
            array[0] = 1l; 
            array[DISTANCE - 1] = 1l;
        } else if (teamID == 1) {
            while (array[DISTANCE - 1] == 0l);
            printf("first element = %ld, last element = %ld\n", array[0], array[DISTANCE - 1]);
        }
    }
    return 0;
}
