#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

struct s {
    float value;
    int pad[NUMP];
};

int main(int argc, char *argv[]) {

    #ifndef _OPENMP
        fprintf(stderr, "OpenMp is not available\n");
        return 1;
    #endif


    unsigned char *p = (unsigned char *)malloc((NUMT+1)*sizeof(struct s));
    int offset = (int)p & 0x3f;
    struct s *Array = (struct s*)&p[64-offset];

    omp_set_num_threads(NUMT);
    long someBigNumber = 1000000000;

    double time0 = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < NUMT; i++){
        for (int j = 0; j < someBigNumber; j++) {
            Array[i].value = Array[i].value + 2;
        }
    }

    double time1 = omp_get_wtime();

    printf("%8.1lf\n", someBigNumber * NUMT / ((time1-time0)*1000000));

    return 0;
}
