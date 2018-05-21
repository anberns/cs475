/* Project 1 implementation
 * Aaron Berns
 * 4/12/18
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define XMIN     0.
#define XMAX     3.
#define YMIN     0.
#define YMAX     3.
#define TOPZ00  0.
#define TOPZ10  1.
#define TOPZ20  0.
#define TOPZ30  0.
#define TOPZ01  1.
#define TOPZ11  6.
#define TOPZ21  1.
#define TOPZ31  0.
#define TOPZ02  0.
#define TOPZ12  1.
#define TOPZ22  0.
#define TOPZ32  4.
#define TOPZ03  3.
#define TOPZ13  2.
#define TOPZ23  3.
#define TOPZ33  3.
#define BOTZ00  0.
#define BOTZ10  -3.
#define BOTZ20  0.
#define BOTZ30  0.
#define BOTZ01  -2.
#define BOTZ11  10.
#define BOTZ21  -2.
#define BOTZ31  0.
#define BOTZ02  0.
#define BOTZ12  -5.
#define BOTZ22  0.
#define BOTZ32  -6.
#define BOTZ03  -3.
#define BOTZ13   2.
#define BOTZ23  -8.
#define BOTZ33  -3.

// float types changed to double for increased precision
// to handle more nodes
double Height( int iu, int iv, int n )
{
	// iu,iv = 0 .. n-1

	double u = (double)iu / (double)(n-1);
	double v = (double)iv / (double)(n-1);

	// the basis functions:
	double bu0 = (1.-u) * (1.-u) * (1.-u);
	double bu1 = 3. * u * (1.-u) * (1.-u);
	double bu2 = 3. * u * u * (1.-u);
	double bu3 = u * u * u;
	double bv0 = (1.-v) * (1.-v) * (1.-v);
	double bv1 = 3. * v * (1.-v) * (1.-v);
	double bv2 = 3. * v * v * (1.-v);
	double bv3 = v * v * v;

	// finally, we get to compute something:
	double top = bu0 * ( bv0*TOPZ00 + bv1*TOPZ01 + bv2*TOPZ02 + bv3*TOPZ03 )
	+ bu1 * ( bv0*TOPZ10 + bv1*TOPZ11 + bv2*TOPZ12 + bv3*TOPZ13 )
	+ bu2 * ( bv0*TOPZ20 + bv1*TOPZ21 + bv2*TOPZ22 + bv3*TOPZ23 )
	+ bu3 * ( bv0*TOPZ30 + bv1*TOPZ31 + bv2*TOPZ32 + bv3*TOPZ33 );
	
	double bot = bu0 * ( bv0*BOTZ00 + bv1*BOTZ01 + bv2*BOTZ02 + bv3*BOTZ03 )
	+ bu1 * ( bv0*BOTZ10 + bv1*BOTZ11 + bv2*BOTZ12 + bv3*BOTZ13 )
	+ bu2 * ( bv0*BOTZ20 + bv1*BOTZ21 + bv2*BOTZ22 + bv3*BOTZ23 )
	+ bu3 * ( bv0*BOTZ30 + bv1*BOTZ31 + bv2*BOTZ32 + bv3*BOTZ33 );

	return top - bot;       
}

int main( int argc, char *argv[ ] )
{
    // get nodes (n) and number of threads
    int n = atoi(argv[1]);
    int threads = atoi(argv[2]);
    
    omp_set_num_threads(threads);

	// the areas of tiles
	double fullTileArea = ( ( ( XMAX - XMIN )/(double)(n-1) ) *
	( ( YMAX - YMIN )/(double)(n-1) )  );

    double halfTileArea = fullTileArea * 0.5;

    double fourthTileArea = fullTileArea * 0.25;

	// sum up the weighted heights into the variable "volume"
	// using an OpenMP for loop and a reduction:

    int i, iu, iv;
    double volume;
    double maxMegaH = 0.;
    double sumMegaH = 0.;
    double sumTime = 0.;
    int numTries = 4;

    // take average of numTries
    for (int t = 0; t < numTries; ++t) {
        volume = 0.;
        double time0 = omp_get_wtime();

        // for loop with reduction
        #pragma omp parallel for reduction(+:volume)
        for (i = 0; i < n * n; ++i) {

            iu = i % n;
            iv = i / n;

            // determine tile type and calculate volume contribution

            // corner
            if (i == 0 || i == (n-1) || i == (n * (n-1)) || i == ((n*n)-1)) {
                volume += (Height(iu, iv, n) * fourthTileArea);
            } 

            // middle, if not an edge
            else if (iu != 0 && iu != (n-1) && iv != 0 && iv != (n-1)) {
                volume += (Height(iu, iv, n) * fullTileArea);
            } 
            
            // edge
            else {
                volume += (Height(iu, iv, n) * halfTileArea);
            }
        }
        double time1 = omp_get_wtime();
        sumTime += (time1 - time0);
        double megah = (((n*n) / (time1 - time0)) / 1000000); // mega-heights
        sumMegaH += megah; 
        if (megah > maxMegaH) maxMegaH = megah;
    }
    // print data
    printf("Nodes: %d, Threads: %d, Volume: %lf, Avg Mega-Heights: %8.2lf, Avg Time: %lf\n", 
            n, threads, volume, sumMegaH / numTries, sumTime / numTries);

	return 0;
}
