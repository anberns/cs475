#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int
main( int argc, char *argv[])
{
#ifndef _OPENMP
        fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
        return 1;
#endif

    float *A, *B, *C;
    int i, j;
    int sArray[] = {1000, 10000, 100000, 100000, 1000000, 10000000, 100000000, 1000000000};
    for (j = 0; j < sizeof(sArray) / sizeof(float); ++j) {

        A = (float *)malloc(sArray[j] * sizeof(float));
        B = (float *)malloc(sArray[j] * sizeof(float));
        C = (float *)malloc(sArray[j] * sizeof(float));

        double time0 = omp_get_wtime( );
        int k;

        for( k = 0; k < sArray[j]; k++ )
        {
                C[k] = A[k] * B[k];
        }

        double time1 = omp_get_wtime( );
        double totalTime = time1-time0; 
        free(A);
        free(B);
        free(C);

        printf( "Size: %d, Time: %2.8lf \n", sArray[j], totalTime);
        }


    return 0;
}
