/*
CS553
Network Benchmarking (By Abhishek Vijhani)
In this program we are basically measuring the network speed over a loopback interface.
In this we have implemented TCP Server and Client and UDP Server and Client.
We have to pass the Port Number and Number of Threads as the parameter of the program.
*/
#include <iostream>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h> 
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>    /* Contains constants and structures needed for internet domain addresses. */
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <arpa/inet.h>


using namespace std;

typedef std::chrono::duration<long, std::ratio<1,1000>> millisecs;   /* 2 Milliseconds */
template <typename T>
long duration(std::chrono::time_point<T> time)
{
  auto differance = std::chrono::system_clock::now() - time;
  return std::chrono::duration_cast<millisecs>( differance ).count();
}

void error(const char *msg)
{
  perror(msg);
  exit(1);
}


int TCPserver(int iterations, int portnum)
{
	int buffsize = 64000;
  socklen_t clilen;
  char buffer[64000];
  struct sockaddr_in serv_addr, cli_addr;
  
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
  {
    error("ERROR Opening Socket");
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  int portno = portnum;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  /* Bind the socket to an address using the bind() system call.*/
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)       
  {
    error("Binding Error");
  }
  /* Listen for connections with the listen() system call*/
  listen(sockfd,5);           
  clilen = sizeof(cli_addr);
   /* Accept a connection with the accept() system call*/
  int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);     
  if (newsockfd < 0) 
  {
    error("Acception Error");
  }
  bzero(buffer,buffsize);
  int i=0;
  while (i < iterations)
  {
  int n = read(newsockfd,buffer,buffsize);
    if (n < 0) 
	{
		error("Reading from Socket Error");
	}
	i =  i + 1;
  }

  close(newsockfd);
  close(sockfd);
  return 0; 
}


int TCPclient(int iterations, int portnum)
{
  struct sockaddr_in serv_addr;
  struct hostent *server;
  int buffsize = 64000;
  char buffer[64000];
  int portno = portnum;
  /* Parameters = address domain of the socket, second argument is the type of socket, The third argument is the protocol. */
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);     
  if (sockfd < 0) 
  {
    error("Error on Opening the Socket");
  }
  server = gethostbyname("127.0.0.1");
  if (server == NULL) 
  {
    fprintf(stderr,"ERROR, There is no such Host\n");
    exit(0);
  }
	/* The function bzero() sets all values in a buffer to zero. It takes two arguments, the first is a pointer to the buffer and the second is the size of the buffer.*/
  bzero((char *) &serv_addr, sizeof(serv_addr));    
	
  serv_addr.sin_family = AF_INET;
	
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);

  serv_addr.sin_port = htons(portno);

	
  if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)  
  {
    error("Error in Connection");
  }
  /*send first message to server to initialize socket on that end   */
  int n = write(sockfd,"Abhishek",8);                           
  cout << "Initialized the Socket" << endl;

  bzero(buffer,buffsize);
  int i = 0;
  while (i<buffsize)
  {
    buffer[i]='A' + random()%26;
	i = i+1;
  }
    
  i = 0;
  while (i<iterations)
  {
    n = write(sockfd,buffer,buffsize);
		if (n < 0) 
		{
		  error("Writing to Socket Error");
		}
	i=i+1;
  }

  bzero(buffer,buffsize);

  close(sockfd);
  return 0;
}



int UDPserver(int iterations, int portnum)
{
  int buffsize = 64000;
  int newsockfd;
  socklen_t clilen;
  char buffer[64000];
  struct sockaddr_in serv_addr, cli_addr;
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) 
  {
    error("ERROR opening socket");
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  int portno = portnum;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
   /* Bind the socket to an address using the bind() system call.*/
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
  {
    error("ERROR on binding");
  }
   /* Listen for connections with the listen() system call*/
  listen(sockfd,5);
  clilen = sizeof(cli_addr);

  bzero(buffer,buffsize);
  int i = 0;
  while (i<iterations)
  {
    int n = recv(sockfd,buffer,buffsize,MSG_WAITALL);
    if (n < 0) 
	{
		error("ERROR reading from socket");
	}
	i = i + 1;
  }

  close(newsockfd);
  close(sockfd);
  return 0; 
}


int UDPclient(int iterations, int portnum)
{
	int buffsize = 64000;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  char buffer[64000];
  int portno = portnum;
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) 
  {
    error("ERROR opening socket");
  }
  server = gethostbyname("127.0.0.1");
  if (server == NULL) 
  {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
  serv_addr.sin_port = htons(portno);
  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
  {
    error("ERROR connecting");
  }
  cout << "socket Has been Initialized" << endl;
  bzero(buffer,buffsize);
  int i = 0;
  while (i<buffsize)
  {
    buffer[i]='A' + random()%26;
	i = i + 1;
  }
  i = 0;
  while (i < iterations)
  {
    int n = send(sockfd,buffer,buffsize,MSG_CONFIRM);
    if (n < 0) 
	{
      error("ERROR writing to socket");
	}
	i = i + 1;
  }

  bzero(buffer,buffsize);

  close(sockfd);
  return sockfd;
}


int main(int argc, char* argv[])
{
  if (argc<3){
    cout << "Enter ./net Port and  NumberofThreads" << endl;
    return 0;
  }

  
  int portnum = atoi(argv[1]);
  int numthreads = atoi(argv[2]);
  int buffsize = 64000;
  int iterations = 10000;

  thread threads[numthreads*2];

  if (numthreads==1){
    cout << "TCP 1 thread" << endl;

    threads[0] = thread(TCPserver,(2*iterations),portnum);
    
    auto beforeTCP = std::chrono::system_clock::now();
 
    threads[1] = thread(TCPclient,iterations,portnum);

    threads[1].join();

    auto time_elapsedTCP = duration(beforeTCP);
    cout << "Latency: "<<time_elapsedTCP << "ms" <<endl;
    ulong latencyTCP = (iterations/time_elapsedTCP) * 1000 / 1000;
	cout << "---------------------------------------------" << endl;   
	cout << "Througput: " <<latencyTCP*buffsize / 8000 << "Mbps" << endl;

    threads[0].join();
  
    cout << "Waiting 10 sec before UDP..." << endl;
    sleep(10);
    cout << "UDP 1 Thread" << endl;

    threads[0] = thread(UDPserver,iterations,portnum);
    
    auto beforeUDP = std::chrono::system_clock::now();
    
    threads[1] = thread(UDPclient,iterations,portnum);

    threads[1].join();
    
    auto time_elapsedUDP = duration(beforeUDP);
	cout << "Latency: "<<time_elapsedUDP << "ms" <<endl;
    ulong latencyUDP = (iterations/time_elapsedUDP) * 1000 / 1000;
	cout << "---------------------------------------------" << endl;
    cout << "Througput: " <<latencyUDP*buffsize / 8000 << "Mbps" << endl;exit(0);

    threads[0].join();
	exit(0);
  }
    else if(numthreads==2){
    cout << "TCP 2 Threads" <<endl;

    threads[0] = thread(TCPserver,iterations,portnum);
    threads[1] = thread(TCPserver,iterations,portnum+1);
    
    auto beforeTCP = std::chrono::system_clock::now();
    
    threads[2] = thread(TCPclient,(iterations/2),portnum);
    threads[3] = thread(TCPclient,(iterations/2),portnum+1);

    threads[3].join();
    threads[2].join();

    auto time_elapsedTCP = duration(beforeTCP);
    cout << "Latency: "<<time_elapsedTCP << "ms" <<endl;
    ulong latencyTCP = (iterations/time_elapsedTCP) * 1000 / 1000;
	cout << "---------------------------------------------" << endl;   
	cout << "Througput: " <<latencyTCP*buffsize / 8000 << "Mbps" << endl;

    threads[1].join();
    threads[0].join();

    cout << "Waiting 15 sec before UDP in attempt to prevent system from refusing connection" << endl;
    sleep(15);
    cout << "UDP 2 Threads" << endl;

    threads[0] = thread(UDPserver,iterations,portnum);
    threads[1] = thread(UDPserver,iterations,portnum+1);
    
    auto beforeUDP = std::chrono::system_clock::now();
    
    threads[2] = thread(UDPclient,(iterations/2),portnum);
    threads[3] = thread(UDPclient,(iterations/2),portnum+1);

    threads[3].join();
    threads[2].join();
    
    auto time_elapsedUDP = duration(beforeUDP);
	cout << "Latency: "<<time_elapsedUDP << "ms" <<endl;
    ulong latencyUDP = (iterations/time_elapsedUDP) * 1000 / 1000;
	cout << "---------------------------------------------" << endl;
    cout << "Througput: " <<latencyUDP*buffsize / 8000 << "Mbps" << endl; exit(0);


    threads[1].join();
    threads[0].join();
	exit(0);
  }
  else if(numthreads==4){
    cout << "TCP 4 Threads" <<endl;

    threads[0] = thread(TCPserver,iterations,portnum);
    threads[1] = thread(TCPserver,iterations,portnum+1);
	threads[2] = thread(TCPserver,iterations,portnum+2);
	threads[3] = thread(TCPserver,iterations,portnum+3);
    
    auto beforeTCP = std::chrono::system_clock::now();
    
    threads[4] = thread(TCPclient,(iterations/4),portnum);
    threads[5] = thread(TCPclient,(iterations/4),portnum+1);
    threads[6] = thread(TCPclient,(iterations/4),portnum+2);
    threads[7] = thread(TCPclient,(iterations/4),portnum+3);

    threads[7].join();
    threads[6].join();
	threads[5].join();
	threads[4].join();

    auto time_elapsedTCP = duration(beforeTCP);
    cout << "Latency: "<<time_elapsedTCP << "ms" <<endl;
    ulong latencyTCP = (iterations/time_elapsedTCP) * 1000 / 1000;
	cout << "---------------------------------------------" << endl;   
	cout << "Througput: " <<latencyTCP*buffsize / 8000 << "Mbps" << endl;

    threads[3].join();
	threads[2].join();
	threads[1].join();
    threads[0].join();

    cout << "Waiting 20 sec before UDP in attempt to prevent system from refusing connection" << endl;
    sleep(20);
    cout << "UDP 4 Threads" << endl;

    threads[0] = thread(UDPserver,iterations,portnum);
    threads[1] = thread(UDPserver,iterations,portnum+1);
	threads[2] = thread(UDPserver,iterations,portnum+2);
	threads[3] = thread(UDPserver,iterations,portnum+3);
    
    auto beforeUDP = std::chrono::system_clock::now();
    
	threads[4] = thread(UDPclient,(iterations/4),portnum);
    threads[5] = thread(UDPclient,(iterations/4),portnum+1);
    threads[6] = thread(UDPclient,(iterations/4),portnum+2);
    threads[7] = thread(UDPclient,(iterations/4),portnum+3);

    threads[7].join();
    threads[6].join();
	threads[5].join();
	threads[4].join();
    
    auto time_elapsedUDP = duration(beforeUDP);
	cout << "Latency: "<<time_elapsedUDP << "ms" <<endl;
    ulong latencyUDP = (iterations/time_elapsedUDP) * 1000 / 1000;
	cout << "---------------------------------------------" << endl;
    cout << "Througput: " <<latencyUDP*buffsize / 8000 << "Mbps" << endl;exit(0);



    threads[3].join();
	threads[2].join();
	threads[1].join();
    threads[0].join();
	exit(0);

  }
  else{
	  cout << "TCP 8 Threads" <<endl;

    threads[0] = thread(TCPserver,iterations,portnum);
    threads[1] = thread(TCPserver,iterations,portnum+1);
	threads[2] = thread(TCPserver,iterations,portnum+2);
	threads[3] = thread(TCPserver,iterations,portnum+3);
	threads[4] = thread(TCPserver,iterations,portnum+4);
	threads[5] = thread(TCPserver,iterations,portnum+5);
	threads[6] = thread(TCPserver,iterations,portnum+6);
	threads[7] = thread(TCPserver,iterations,portnum+7);
    
    auto beforeTCP = std::chrono::system_clock::now();
    
    threads[8] = thread(TCPclient,(iterations/8),portnum);
    threads[9] = thread(TCPclient,(iterations/8),portnum+1);
    threads[10] = thread(TCPclient,(iterations/8),portnum+2);
    threads[11] = thread(TCPclient,(iterations/8),portnum+3);
	threads[12] = thread(TCPclient,(iterations/8),portnum+4);
	threads[13] = thread(TCPclient,(iterations/8),portnum+5);
	threads[14] = thread(TCPclient,(iterations/8),portnum+6);
	threads[15] = thread(TCPclient,(iterations/8),portnum+7);

    threads[15].join();
    threads[14].join();
	threads[13].join();
	threads[12].join();
	threads[11].join();
	threads[10].join();
	threads[9].join();
	threads[8].join();

    auto time_elapsedTCP = duration(beforeTCP);
    cout << "Latency: "<<time_elapsedTCP << "ms" <<endl;
    ulong latencyTCP = (iterations/time_elapsedTCP) * 1000 / 1000;
	cout << "---------------------------------------------" << endl;   
	cout << "Througput: " <<latencyTCP*buffsize / 8000 << "Mbps" << endl;

    threads[7].join();
	threads[6].join();
	threads[5].join();
	threads[4].join();
	threads[3].join();
	threads[2].join();
	threads[1].join();
    threads[0].join();

    cout << "Waiting 20 sec before UDP in attempt to prevent system from refusing connection" << endl;
    sleep(20);
    cout << "UDP 8 Threads" << endl;

    threads[0] = thread(UDPserver,iterations,portnum);
    threads[1] = thread(UDPserver,iterations,portnum+1);
	threads[2] = thread(UDPserver,iterations,portnum+2);
	threads[3] = thread(UDPserver,iterations,portnum+3);
	threads[4] = thread(UDPserver,iterations,portnum+4);
	threads[5] = thread(UDPserver,iterations,portnum+5);
	threads[6] = thread(UDPserver,iterations,portnum+6);
	threads[7] = thread(UDPserver,iterations,portnum+7);
    
    auto beforeUDP = std::chrono::system_clock::now();
    
	threads[8] = thread(UDPclient,(iterations/8),portnum);
    threads[9] = thread(UDPclient,(iterations/8),portnum+1);
    threads[10] = thread(UDPclient,(iterations/8),portnum+2);
    threads[11] = thread(UDPclient,(iterations/8),portnum+3);
	threads[12] = thread(UDPclient,(iterations/8),portnum+4);
	threads[13] = thread(UDPclient,(iterations/8),portnum+5);
	threads[14] = thread(UDPclient,(iterations/8),portnum+6);
	threads[15] = thread(UDPclient,(iterations/8),portnum+7);

    threads[15].join();
    threads[14].join();
	threads[13].join();
	threads[12].join();
	threads[11].join();
	threads[10].join();
	threads[9].join();
	threads[8].join();
    
    auto time_elapsedUDP = duration(beforeUDP);
	cout << "Latency: "<<time_elapsedUDP << "ms" <<endl;
    ulong latencyUDP = (iterations/time_elapsedUDP) * 1000 / 1000;
	cout << "---------------------------------------------" << endl;
    cout << "Througput: " <<latencyUDP*buffsize / 8000 << "Mbps" << endl;exit(0);


    threads[7].join();
	threads[6].join();
	threads[5].join();
	threads[4].join();
	threads[3].join();
	threads[2].join();
	threads[1].join();
    threads[0].join();
	exit(0);
  }
  exit(0);
}
