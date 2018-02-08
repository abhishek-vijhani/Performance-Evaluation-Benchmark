/*
CS553
CPU Benchmarking (By Abhishek Vijhani)
In the CPU Benchmarking we are measuring the processor speed of GIOPS and GFLOPS. using the AVX Instructions.
Measuring the Processor Speed using various threads (1,2,4,8).
We have to pass Number of threads as the parameter of the program.
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<pthread.h>
#include<immintrin.h>
#include<limits.h>

long long ITR = INT_MAX;
struct thread_args {
    long long total;
};

void *compute_flops(void *arguments);
void *compute_iops(void *arguments);

int main( int argc, char *argv[])
{
	clock_t start_time,end_time, s_t, e_t;
	int choice = atoi(argv[1]);
	pthread_t *pt;
	pt = (pthread_t*)malloc(sizeof(pthread_t)*8); 
	char str[32];
	
		
		if(choice == 0){
			exit(0);
		}
		
		struct thread_args args;
		long long total = ITR / choice;
		args.total = total;

		int i=0;
		start_time = clock();
		for(i=0; i < choice; i++)
		{
			pthread_create(&pt[i],NULL,compute_flops, (void *)&args);
		}
		for(i=0; i < choice; i++)
		{
			pthread_join(pt[i],NULL);	
		}
		end_time = clock();
		
		double time_taken = (double)(end_time - start_time) / (CLOCKS_PER_SEC);
		printf("\nGFLOPS for %d Thread: %f\n", choice, (((12 * ITR)/time_taken)/1000000000));
		
		
		
		start_time = clock();
		for(i=0; i < choice; i++)
		{
			pthread_create(&pt[i],NULL,compute_iops, (void *)&args); 
		}
		for(i=0; i < choice; i++)
		{
			pthread_join(pt[i],NULL);
		}
		end_time = clock();
		time_taken = (double)(end_time - start_time) / (CLOCKS_PER_SEC);
		printf("\nGILOPS for %d Thread: %f\n\n\n", choice, (((12 * ITR)/time_taken)/1000000000));
		
		return 0;


}
void *compute_flops(void *arguments)
{
	struct thread_args *args = (struct thread_args *)arguments;
	long long total = args -> total;
	__m256d a = _mm256_set_pd(2.0, 4.0, 6.0, 8.0);
    __m256d b = _mm256_set_pd(1.0, 3.0, 5.0, 7.0);
    
	__m256d c = _mm256_set_pd(2.0, 4.0, 6.0, 8.0);
    __m256d d = _mm256_set_pd(1.0, 3.0, 5.0, 7.0);
    
	__m256d e = _mm256_set_pd(2.0, 4.0, 6.0, 8.0);
    __m256d f = _mm256_set_pd(1.0, 3.0, 5.0, 7.0); 
	int i;
	for(i=0; i < total; i++){
		__m256d result = _mm256_add_pd(a, b);
		result = _mm256_add_pd(c, d);
		result = _mm256_add_pd(e, f);
	}
}

void *compute_iops(void *arguments)
{
	struct thread_args *args = (struct thread_args *)arguments;
	long long total = args -> total;
	__m256i a = _mm256_set_epi64x(1, 2, 3, 4);
  	__m256i b = _mm256_set_epi64x(5, 6, 7, 8);
  	
  	__m256i c = _mm256_set_epi64x(1, 2, 3, 4);
  	__m256i d = _mm256_set_epi64x(5, 6, 7, 8);
  	
  	__m256i e = _mm256_set_epi64x(1, 2, 3, 4);
  	__m256i f = _mm256_set_epi64x(5, 6, 7, 8);
	int i;
	for(i=0; i < total; i++){
		__m256i result = _mm256_add_epi64(a, b);
		result = _mm256_add_epi64(c, d);
		result = _mm256_add_epi64(e, f);
	}
}
