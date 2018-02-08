/*
CS553
CPU Benchmarking (By Teja Maripuri)

In the Memory Benchmarking we are measuring the memory latency and throughput using varying block sizes and the varying concurrency.
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<string.h>
#include<memory.h>


int b = 1;
int kb = 1024;
int mb = 1024 * 1024;
int gb = 1024 * 1024 * 1024;
char buff_mb[8388608];
char buff_gb[1024 * 1024 * 1024];
char types[5][20] = {"8 Byte Block", "8 Kilo Bytes Block", "8 Mega Bytes Block", "80 Mega Bytes Block"};
int blocks[] = {8*1, 8*1024, 8*1048576, 80*1048576};

int threads[] = {1, 2, 4, 8};
clock_t start_time,end_time;
 

FILE *fp;

struct thread_args {
    long long total;
    long block;
    int curr_part;
    int flag;
};



void *write(void *arguments);
void *read(void *arguments);
void *read_write(void *arguments);

void *write_random(void *arguments);
void *read_random(void *arguments);

int main()
{
	pthread_t *pt;
	pt = (pthread_t*)malloc(sizeof(pthread_t)*8);
	int itr;
	for (itr = 0; itr < 4; itr++)
	{
		printf("\n\nSequential Read/Write for %s\n\n", types[itr]);
		int t_iter;
		
		for (t_iter = 0; t_iter < 4; t_iter++)
		{
			if(itr == 0)
			{
				int i;
				//printf("Throughput ");
				struct thread_args args;
				long long trueTotal = 8*mb;
				args.total = trueTotal/threads[t_iter];
				args.block = blocks[itr];
				args.flag = 0;
				start_time = clock();
				for (i=0; i< threads[t_iter]; i++)
				{
					args.curr_part = i;
					pthread_create(&pt[i],NULL, &read_write, (void *)&args);
				}
				for (i=0; i< threads[t_iter]; i++)
				{
					pthread_join(pt[i],NULL);
				}
				end_time = clock();
				double time_taken = (double)(end_time - start_time) / 10000000;
				printf("The Latency for Write+Read with %d thread(s): %f us\n", threads[t_iter], time_taken);
				
			}
			else
			{
				int i;
				//printf("Throughput ");
				struct thread_args args;
				long long trueTotal = 1*gb;
				args.total = trueTotal/threads[t_iter];
				args.block = blocks[itr];
				args.flag = 1;
				start_time = clock();
				for (i=0; i< threads[t_iter]; i++)
				{
					args.curr_part = i;
					pthread_create(&pt[i],NULL, &read_write, (void *)&args);
				}
				for (i=0; i< threads[t_iter]; i++)
				{
					pthread_join(pt[i],NULL);
				}
				end_time = clock();
				double speed = (double) (trueTotal/mb) / ((end_time - start_time) / CLOCKS_PER_SEC) ;
				printf("The Throughput for Write+Read with %d thread(s): %.2f Mbps\n", threads[t_iter], speed * 100);
			
				start_time = clock();
				for (i=0; i< threads[t_iter]; i++)
				{
					args.curr_part = i;
					pthread_create(&pt[i],NULL, &write, (void *)&args);
				}
				for (i=0; i< threads[t_iter]; i++)
				{
					pthread_join(pt[i],NULL);
				}
				end_time = clock();
				speed = (double) (trueTotal/mb) / ((end_time - start_time) / CLOCKS_PER_SEC) ;
				printf("The Throughput for Write      with %d thread(s): %.2f Mbps\n\n", threads[t_iter], speed * 100); 
			}
		}
	}
	
	for (itr = 0; itr < 4; itr++)
	{
		printf("\n\nRandom Read/Write for %s\n\n", types[itr]);
		int t_iter;
		
		for (t_iter = 0; t_iter < 4; t_iter++)
		{
			if(itr == 0)
			{
				int i;
				//printf("Throughput ");
				struct thread_args args;
				long trueTotal = 8*kb;
				args.total = trueTotal/threads[t_iter];
				args.block = blocks[itr];
				args.flag = 0;
				start_time = clock();
				for (i=0; i< threads[t_iter]; i++)
				{
					args.curr_part = i;
					pthread_create(&pt[i],NULL, &write_random, (void *)&args);
				}
				for (i=0; i< threads[t_iter]; i++)
				{
					pthread_join(pt[i],NULL);
				}
				end_time = clock();
				double time_taken = (double)(end_time - start_time)  / 10000000;
				printf("The Latency for Write with %d thread(s): %f us\n", threads[t_iter], time_taken);
			}
			else
			{
				int i;
				//printf("Throughput ");
				struct thread_args args;
				long trueTotal = 1*gb;
				args.total = trueTotal/threads[t_iter];
				args.block = blocks[itr];
				args.flag = 1;
				start_time = clock();
				for (i=0; i< threads[t_iter]; i++)
				{
					args.curr_part = i;
					pthread_create(&pt[i],NULL, &write_random, (void *)&args);
				}
				for (i=0; i< threads[t_iter]; i++)
				{
					pthread_join(pt[i],NULL);
				}
				end_time = clock();
				double speed = (double) (trueTotal/mb) / ((end_time - start_time) / CLOCKS_PER_SEC) ;
				printf("The Throughput for Write with %d thread(s): %.2f Mbps\n", threads[t_iter], speed * 100);
			
				
			}
		}
	}
	return 0;
}

void *write(void *arguments)
{
	struct thread_args *args = (struct thread_args *)arguments;
	long total = args -> total;
	long block = args -> block;
	int curr_part = args -> curr_part;
	int flag = args -> flag;
	long i, j;
	long iter = total/block;
	for(j=0; j < 100; j++)
	{
		for(i=0; i < iter; i++)
		{
			memset(&buff_gb[((curr_part * total) + (i*block))], 'a', block);
		}
	}
	
}

void *read_write(void *arguments)
{
	struct thread_args *args = (struct thread_args *)arguments;
	long total = args -> total;
	long block = args -> block;
	int curr_part = args -> curr_part;
	int flag = args -> flag;
	char *item = NULL;  
	item = malloc (total * sizeof *item);
	long i, j;
	long iter = total/block;
	for(j=0; j < 100; j++)
	{
		for(i=0; i < iter; i++)
		{
			if(flag == 1)
			{
				memcpy(&buff_gb[((curr_part * total) + (i*block))],&item[(i*block)], block);
			}
			else
			{
 				memcpy(&buff_mb[((curr_part * total) + (i*block))],&item[(i*block)], block);
			}
		}
	}
	
	free(item);
}



void *write_random(void *arguments)
{
	struct thread_args *args = (struct thread_args *)arguments;
	long total = args -> total;
	long block = args -> block;
	int curr_part = args -> curr_part;
	int flag = args -> flag;
	long i;
	long j;
	long iter = total/block;
	for(j=0; j < 100; j++)
	{
		for(i=0; i < iter; i++)
		{
			if(flag == 1)
			{
				long rand_num = rand() % iter;
				memset(&buff_gb[((curr_part * total) + (rand_num*block))], 'a', block);
			}
			else
			{
				long rand_num = rand() % iter;
				memset(&buff_mb[((curr_part * total) + (rand_num*block))], 'a', block);
			}
	}
	}
	
}
