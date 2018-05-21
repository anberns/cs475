project5 : project5.c
	icc -o project5 project5.c -lm -fopenmp -no-vec

project5reduc : project5reduc.c
	icc -o project5reduc project5reduc.c -lm -fopenmp -no-vec

project5simd : project5simd.cpp simd.p5.cpp
	g++ -o project5simd project5simd.cpp simd.p5.cpp -lm -fopenmp

test: test.cpp simd.p5.cpp
	icpc -o test test.cpp simd.p5.cpp 
