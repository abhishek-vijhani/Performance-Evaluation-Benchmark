/*
CS553
CPU Benchmarking (By Teja Maripuri)

In the Disk Benchmarking we are measuring the disk speed on the varying block sizes and the varying concurrency
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<string.h>

int b = 1;
int kb = 1024;
int mb = 1024 * 1024;
int gb = 1024 * 1024 * 1024;
//long long f_size = 10737418240ULL;
long long f_size = 1024 * 1024 * 1024;
char *buff;
char types[5][20] = {"8 Byte Block", "8 Kilo Bytes Block", "8 Mega Bytes Block", "80 Mega Bytes Block"};
int blocks[] = {8*1, 8*1024, 8*1048576, 80*1048576};

int threads[] = {1, 2, 4, 8};
clock_t start_time,end_time;
 

FILE *fp, *fp1;

struct thread_args {
    long long total;
    long block;
    int curr_part;
    FILE *fp;
};



void *read_write(void *arguments);
void *read(void *arguments);
void write_first(FILE *fp, long long f_size);

void *write_random(void *arguments);
void *read_random(void *arguments);

int main()
{
	fp1 = fopen("test1.txt", "w+");
	write_first(fp1, f_size);
	fclose(fp1);
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
				long long trueTotal = 8*kb;
				args.total = trueTotal/threads[t_iter];
				args.block = blocks[itr];
				fp = fopen("test.txt", "w+");
				args.fp = fp;
				start_time = clock();
				int k = 0;
				for (k=0; k< 10; k++)
				{
					for (i=0; i< threads[t_iter]; i++)
				{
					args.curr_part = i;
					pthread_create(&pt[i],NULL, &read_write, (void *)&args);
				}
				for (i=0; i< threads[t_iter]; i++)
				{
					pthread_join(pt[i],NULL);
				}
				}
				
				end_time = clock();
				fclose(fp);
				double time_taken = (double)(end_time - start_time) / 10;
				printf("The Latency for Read+Write with %d thread(s): %.3f ms\n", threads[t_iter], time_taken/1000);
			}
			else
			{
				int i;
				//printf("Throughput ");
				struct thread_args args;
				long long trueTotal = f_size;
				args.total = trueTotal/threads[t_iter];
				args.block = blocks[itr];
				fp = fopen("test.txt", "w+");
				args.fp = fp;
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
				fclose(fp);
				//double speed = (double)(threads[t_iter] * (args.total/args.block))/mb/((end_time - start_time) / CLOCKS_PER_SEC);
				double time_taken = (double)(end_time - start_time)/ CLOCKS_PER_SEC;
				double speed = (trueTotal/mb) / time_taken ;
				printf("The Throughput for Read+Write with %d thread(s): %.2f Mbps\n", threads[t_iter], speed);
			
				fp = fopen("test1.txt", "r");
				args.fp = fp;
				start_time = clock();
				for (i=0; i< threads[t_iter]; i++)
				{
					args.curr_part = i;
					pthread_create(&pt[i],NULL, &read, (void *)&args);
				}
				for (i=0; i< threads[t_iter]; i++)
				{
					pthread_join(pt[i],NULL);
				}
				end_time = clock();
				fclose(fp);
				time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
				speed = (double) (trueTotal/mb) / time_taken ;
				printf("The Throughput for Read  with %d thread(s): %.2f Mbps\n\n", threads[t_iter], speed); 
			}
		}
	}
	
	for (itr = 0; itr < 4; itr++)
	{
		printf("Random Read for %s\n\n", types[itr]);
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
			
				fp = fopen("test1.txt", "r");
				args.fp = fp;
				start_time = clock();
				int k = 0;
				for (k=0; k< 10; k++)
				{
					for (i=0; i< threads[t_iter]; i++)
				{
					args.curr_part = i;
					pthread_create(&pt[i],NULL, &read_random, (void *)&args);
				}
				for (i=0; i< threads[t_iter]; i++)
				{
					pthread_join(pt[i],NULL);
				}
				}
				
				end_time = clock();
				fclose(fp);
				double time_taken1 = (double)(end_time - start_time) / 10;
				printf("The Latency for Read  with %d thread(s): %.3f ms\n", threads[t_iter], time_taken1/1000);
			}
			else
			{
				int i;
				//printf("Throughput ");
				struct thread_args args;
				long trueTotal = f_size;
				args.total = trueTotal/threads[t_iter];
				args.block = blocks[itr];
				
				fp = fopen("test1.txt", "r");
				args.fp = fp;
				start_time = clock();
				for (i=0; i< threads[t_iter]; i++)
				{
					args.curr_part = i;
					pthread_create(&pt[i],NULL, &read_random, (void *)&args);
				}
				for (i=0; i< threads[t_iter]; i++)
				{
					pthread_join(pt[i],NULL);
				}
				end_time = clock();
				fclose(fp);
				double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
				double speed = (double) (trueTotal/mb) / time_taken ;
				printf("The Throughput for Read  with %d thread(s): %.2f Mbps\n\n", threads[t_iter], speed); 
			}
		}
	}
	return 0;
}

void write_first(FILE* fp, long long f_size)
{
	long block = (100 * mb);
	char *item = NULL;  
	item = malloc (block * sizeof *item);
	long long i;
	int p_size = (int) f_size / block;
	for(i = 0; i < p_size; i++)
	{
		fseek(fp, i * block,SEEK_SET);
    	fwrite(item,block,1,fp);
	}
	free(item);
	
}

void *read_write(void *arguments)
{
	struct thread_args *args = (struct thread_args *)arguments;
	long total = args -> total;
	long block = args -> block;
	int curr_part = args -> curr_part;
	FILE *fp = args -> fp;
	char *item = NULL;  
	FILE *fp1;
	fp1 = fopen("test1.txt", "r");
	item = malloc (block * sizeof *item);
	//fp = fopen("test.txt", "w+");
	long i;
	long iter = total/block;
	for(i=0; i < iter; i++)
	{
		fseek(fp1,((curr_part * total) + (i*block)),SEEK_SET);
    	fread(item,block,1,fp1);
		fseek(fp,((curr_part * total) + (i*block)),SEEK_SET);
    	fwrite(item,block,1,fp);
	}
	//fclose(fp);
	free(item);
}

void *read(void *arguments)
{
	struct thread_args *args = (struct thread_args *)arguments;
	long total = args -> total;
	long block = args -> block;
	int curr_part = args -> curr_part;
	FILE *fp = args -> fp;
	char *item = NULL;  
	item = malloc (block * sizeof *item);
	//char item[block];
	//fp = fopen("test.txt", "r");
	long i;
	long iter = total/block;
	for(i=0; i < iter; i++)
	{
		fseek(fp,((curr_part * total) + (i*block)),SEEK_SET);
    	fread(item,block,1,fp);
	}
	//fclose(fp);
	free(item);
}

void *write_random(void *arguments)
{
	struct thread_args *args = (struct thread_args *)arguments;
	long total = args -> total;
	long block = args -> block;
	int curr_part = args -> curr_part;
	FILE *fp = args -> fp;
	char *item = NULL;  
	item = malloc (block * sizeof *item);
	//char item[block];
	//fp = fopen("test.txt", "w+");
	long i;
	long iter = total/block;
	for(i=0; i < iter; i++)
	{
		long rand_num = rand() % iter;
		fseek(fp,((curr_part * total) + (rand_num*block)),SEEK_SET);
    	fwrite(item,block,1,fp);
	}
	//fclose(fp);
	free(item);
}

void *read_random(void *arguments)
{
	struct thread_args *args = (struct thread_args *)arguments;
	long total = args -> total;
	long block = args -> block;
	int curr_part = args -> curr_part;
	FILE *fp = args -> fp;
	char *item = NULL;  
	item = malloc (block * sizeof *item);
	//char item[block];
	//fp = fopen("test.txt", "r");
	long i;
	long iter = total/block;
	for(i=0; i < iter; i++)
	{
		long rand_num = rand() % iter;
		fseek(fp,((curr_part * total) + (rand_num*block)),SEEK_SET);
    	fread(item,block,1,fp);
	}
	//fclose(fp);
	free(item);
}
