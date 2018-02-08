# Performance-Evaluation-Benchmark
Performance Evaluation Benchmark

This Benchmark need GCC, G++ Compiler and AVX Libraries for CPU Benchmarking and Pthread Library for all Benchmarkings.

#For Comiling all the Benchmarks, use the command:
	make all

#For Removing the binary files from the system, use the command:
	make clean
		
#This will compile disk and memory the programs for the benchmark and will execute the programs.

#If faced with Binding Error/Already in Use Error for Newtwork Benchmarking. Please run the program separately with different port numbers.

#To execute the network code, we have to enter 2 parameters: Port Number and Number of Threads. As instructed in the assignment we have fixed the 64Kb as number of bytes and the data size which will be equal to approximately 640MB which is equal to 10,000 iterations.
The port is open to be changed to account for firewalls or anything else. We have executed our program at port number 49294 successfully.
We can enter 4 values of threads: 1, 2, 4, 8. and then they will automatically divide the work and call the TCP and UDP.

#To execute the CPU code, we have to enter 1 parameter which is Number of threads. We can enter (1,2,4,8) respectively.

#To run the CPU, Memory, and Disk and Network benchmarks independently, the following commands can be used, respectively:
	make cpu
	make mem
	make disk	
	make net
