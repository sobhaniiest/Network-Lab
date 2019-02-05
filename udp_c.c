#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include <time.h> 
#include <sys/time.h>   
#include <sys/resource.h> 

#define MAX_SIZE 100
#define PORT 8082

char buffer[MAX_SIZE];
char *message="Hello Server from Client\n";
char *server;

int main(int argc, char const *argv[])
{
	int bind_status;
	int sockfd;
	double time;
	struct timespec start={0,0}, end={0,0};
	
	struct sockaddr_in server_address;

	sockfd = socket(AF_INET, SOCK_DGRAM,0);
	if(sockfd < 0)
	{
		perror("Can not create socket");
		exit(0);
	}

	memset(&server_address, 0, sizeof(server_address)); 
	 
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_address.sin_port = htons(PORT);
	int length = sizeof(server_address);
	int n,test;
	test = 10;
	
	clock_gettime(CLOCK_MONOTONIC, &start);
	while(test--)
	{
		sendto(sockfd,(const char*)message,strlen(message),MSG_CONFIRM,(const struct sockaddr*)&server_address,length);
		//printf("Message sent to the Server\n");
		//len = sizeof(server_address);
		n = recvfrom(sockfd,(char*)buffer,MAX_SIZE,MSG_WAITALL,(struct sockaddr*)&server_address,&length);
		buffer[n]='\0';
		//printf("Server>> %s\n", buffer);
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	time = ((double)end.tv_sec*1.0e9 + end.tv_nsec) - ((double)start.tv_sec*1.0e9 + start.tv_nsec);
	printf("Time = %lf ns\n",time/10.0);


	close(sockfd);
	return 0;
}