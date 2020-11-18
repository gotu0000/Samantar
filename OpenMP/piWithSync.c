/*

This program will numerically compute the integral of

                  4/(1+x*x) 
				  
from 0 to 1.  The value of this integral is pi -- which 
is great since it gives us an easy way to check the answer.

The is the original sequential program.  It uses the timer
from the OpenMP runtime library

History: Written by Tim Mattson, 11/99.
gcc -fopenmp piWithSync.c
*/
#include <stdio.h>
#include <omp.h>
static long num_steps = 100000000;
double step;
#define NUM_THREADS 8
#define PAD 8
int main ()
{
	#if 0
	int i;
	double x, pi, sum = 0.0;
	double start_time, run_time;
	step = 1.0/(double) num_steps; 
	start_time = omp_get_wtime();

	for (i=1;i<= num_steps; i++){
		x = (i-0.5)*step;
		sum = sum + 4.0/(1.0+x*x);
	}

	pi = step * sum;
	run_time = omp_get_wtime() - start_time;
	printf("\n pi with %ld steps is %lf in %lf seconds\n ",num_steps,pi,run_time);
	#else
	int threadCounter = 0;
	double pi, sum = 0.0;
	double start_time, run_time;
	int numThreads = NUM_THREADS;
	step = 1.0/(double) num_steps;

	omp_set_num_threads(numThreads);

	printf("\n Number of threads requested = %d\n ",numThreads);

	start_time = omp_get_wtime();
	#pragma omp parallel
	{
		int iD;
		int i;
		int nThreads;
		double x;
		double sumPart;
		iD = omp_get_thread_num();

		nThreads = omp_get_num_threads();
		if(iD == 0)
		{
			numThreads = nThreads;
		}
		for (i=iD;i< num_steps; i=i+nThreads){
		  x = (i+0.5)*step;
		  sumPart = sumPart + 4.0/(1.0+x*x);
		}
		#pragma omp critical
		{
			sum = sum + sumPart;
		}
	}

	pi = step * sum;
	run_time = omp_get_wtime() - start_time;
	printf("\n pi with %ld steps is %lf in %lf seconds\n ",num_steps,pi,run_time);
	#endif
}
