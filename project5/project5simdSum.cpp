#include "simd.p5.h"
#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <sys/time.h>
#include <sys/resource.h>

int main() {

    float *A, *B;
    int i;
    double time0, time1;
    int sArray[] = {1000, 10000,  100000, 1000000, 10000000, 100000000, 1000000000};
    float sum;
    for (i = 0; i < 7; ++i) {

        sum = 0;
        A = new float[sArray[i]];
        B = new float[sArray[i]];

        time0 = omp_get_wtime();

        sum = SimdMulSum(A,B, sArray[i]);
        

        time1 = omp_get_wtime();
        double totalTime = time1 - time0;
        delete [] A;
        delete [] B;

        std::cout << " Size: " <<  sArray[i] << " Time: " <<  totalTime << "\n";
    }

    return 0;
}
