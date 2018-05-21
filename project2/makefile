n_body_coarse_static: n_body_coarse_static.c
	g++-7 -std=c99 -o n_body_coarse_static n_body_coarse_static.c -O3 -lm -fopenmp

n_body_fine_static: n_body_fine_static.c
	g++-7 -std=c99 -o n_body_fine_static n_body_fine_static.c -O3 -lm -fopenmp

n_body_coarse_dynamic: n_body_coarse_dynamic.c
	g++-7 -std=c99 -o n_body_coarse_dynamic n_body_coarse_dynamic.c -O3 -lm -fopenmp

n_body_fine_dynamic: n_body_fine_dynamic.c
	g++-7 -std=c99 -o n_body_fine_dynamic n_body_fine_dynamic.c -O3 -lm -fopenmp

all: n_body_coarse_static n_body_fine_static n_body_coarse_dynamic n_body_fine_dynamic

clean:
	rm n_body_coarse_static n_body_coarse_dynamic n_body_fine_static n_body_fine_dynamic
