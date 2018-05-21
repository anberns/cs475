#include "simd.p5.h"
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <sys/time.h>
#include <sys/resource.h>

int main() {

    /*
    float *A;
    float *B;
    float *C;
    */

    float A[100];
    float B[100];
    float C[100];

    time_t time0, time1;
    int size = 100;

    /*
    A = new float[size];
    B = new float[size];
    C = new float[size];
    */

    time (&time0);

    //SimdMul(A, B, C, size);
    float sum = SimdMulSum(A, B, size);

    time (&time1);
    time_t total = difftime(time1,time0);

    std::cout << " Size: " <<  size << " Time: " <<  total << " Sum: " << sum << "\n";

    return 0;
}
