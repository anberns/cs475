#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

struct s {
    float value;
};

int main(int argc, char *argv[]) {

    #ifndef _OPENMP
        fprintf(stderr, "OpenMp is not available\n");
        return 1;
    #endif


    struct s Array[NUMT];

    omp_set_num_threads(NUMT);
    long someBigNumber = 1000000000;

    double time0 = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < NUMT; i++){
        float temp = Array[i].value;
        for (int j = 0; j < someBigNumber; j++) {
            temp = temp + 2;
        }
        Array[i].value = temp;
    }

    double time1 = omp_get_wtime();

    printf("%lf\n", someBigNumber * NUMT / ((time1-time0)*1000000));

    return 0;
}
