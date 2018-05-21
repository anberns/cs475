/* Aaron Berns
 * 5/10/18
 *
 * grain.c implements the functional decomposition project
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

const float GRAIN_GROWS_PER_MONTH = 8.0;
const float ONE_DEER_EATS_PER_MONTH = 0.5;
const float AVG_PRECIP_PER_MONTH = 6.0;    // average
const float AMP_PRECIP_PER_MONTH = 6.0;    // plus or minus
const float RANDOM_PRECIP = 2.0;    // plus or minus noise
const float AVG_TEMP = 50.0;   // average
const float AMP_TEMP = 20.0;   // plus or minus
const float RANDOM_TEMP = 10.0;   // plus or minus noise
const float MIDTEMP = 40.0;
const float MIDPRECIP = 10.0;

float SQR(float x) {
	return x*x;
}

float Ranf( unsigned int *seedp,  float low, float high ) {
	float r = (float) rand_r( seedp );              // 0 - RAND_MAX
	return(low + r * ( high - low ) / (float)RAND_MAX);
}

int Rani( unsigned int *seedp, int ilow, int ihigh )
{
	float low = (float)ilow;
	float high = (float)ihigh + 0.9999f;
	return (int)( Ranf(seedp, low,high) );
}

void Watcher(int *NowYear, int *NowMonth, float *NowTemp, float *NowPrecip, float *NowHeight, int *NowNumDeer) {
	float ang, temp, precip;
	unsigned int seed = 0;

	while(*NowYear < 2023) {

		// compute barrier
		#pragma omp barrier

		// update barrier
		#pragma omp barrier

		// GMOGrain adjusts NowHeight 
		#pragma omp barrier

		//print stats
		printf("Year: %d, Month: %d, Temp: %3.2f, Precip: %3.2f, Height: %3.2f, Deer: %d\n", *NowYear, *NowMonth, *NowTemp, *NowPrecip, *NowHeight, *NowNumDeer);
// increment month, year
		if (*NowMonth < 11) {
			++*NowMonth;
		} else {
			++*NowYear;
			*NowMonth = 0;
		}

		// update temp and precip
		ang = (30. * (float)(*NowMonth) + 15.) * (M_PI / 180.);
		temp = AVG_TEMP - AMP_TEMP * cos(ang);
		precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin(ang);
		*NowTemp = temp + Ranf(&seed, -RANDOM_TEMP, RANDOM_TEMP);
		*NowPrecip = precip + Ranf(&seed, -RANDOM_PRECIP, RANDOM_PRECIP);
		if (*NowPrecip < 0.) {
			*NowPrecip = 0;
		}

		// wait
		#pragma omp barrier
	}

}
void Grain(int *NowYear, float *NowTemp, float *NowPrecip, float *NowHeight, int *NowNumDeer) {
	float tempFactor, precipFactor, tempHeight;

	while (*NowYear < 2023) {

		// compute tempHeight from current state
		tempFactor = exp(-SQR((*NowTemp - MIDTEMP)/10.));
		precipFactor = exp(-SQR((*NowPrecip - MIDPRECIP)/10.));
		tempHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
		tempHeight -= (float)(*NowNumDeer) * ONE_DEER_EATS_PER_MONTH;

        if (tempHeight < 0) {
            tempHeight = 0;
        }
	
		// DoneComputing barrier
		#pragma omp barrier
		
		// Assign new value and wait
		*NowHeight = tempHeight;
		#pragma omp barrier

		// GMOGrain adjusts NowHeight 
		#pragma omp barrier

		// DonePrinting barrier
		#pragma omp barrier
	}
}

void GrainDeer(int *NowYear, float *NowHeight, int *NowNumDeer) {
	
	int tempDeer;

	while (*NowYear < 2023) {

		// compute new number of deer
		tempDeer = *NowNumDeer;

		if (*NowHeight > tempDeer) {
			++tempDeer;
		} else if (*NowHeight < tempDeer) {
			tempDeer--;
		}

		// DoneComputing barrier
		#pragma omp barrier

		// Assign new value and wait
		*NowNumDeer = tempDeer;
		#pragma omp barrier
		
		// GMOGrain adjusts NowHeight 
		#pragma omp barrier

		// DonePrinting barrier
		#pragma omp barrier
	}
}

void GMOGrain(int *NowYear, float *NowHeight) {

    float growthFactor;
    unsigned int seed = 0;
    
    while (*NowYear < 2023) {

        if (*NowYear == 2019 || *NowYear == 2021) {
            growthFactor = Ranf(&seed, 1.25, 1.75);
        }
        else {
            growthFactor = 1;
        }

        // DoneComputing barrier
        #pragma omp barrier

        // Other threads assign new values
        #pragma omp barrier

        // GMOGrain adjusts NowHeight
        *NowHeight *= growthFactor;
        #pragma omp barrier

        // DonePrinting barrier
        #pragma omp barrier
    }
}

int main(int argc, char *argv[]) {

#ifndef _OPENMP
	fprintf(stderr, "OpenMp is not available\n");
	return 1;
#endif

	int NowYear = 2017;  // 2017 - 2022
	int NowMonth = 0; //0-11
	float NowPrecip; // inches of rain per month
	float NowTemp; // temperature this month
	float NowHeight = 1.; // grain height in inches
	int NowNumDeer = 1; // number of deer in the current population

    omp_set_num_threads(4); 

	#pragma omp parallel sections
	{
		#pragma omp section
		{
			GrainDeer(&NowYear, &NowHeight, &NowNumDeer);
		}
		#pragma omp section
		{
			Grain(&NowYear, &NowTemp, &NowPrecip, &NowHeight, &NowNumDeer); 
		}
		#pragma omp section
		{
			Watcher(&NowYear, &NowMonth, &NowTemp, &NowPrecip, &NowHeight, &NowNumDeer);
		}
		#pragma omp section
        {
            GMOGrain(&NowYear, &NowHeight);
		}
	} // implied barrier

    return 0;
}
