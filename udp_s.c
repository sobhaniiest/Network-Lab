#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define MAX_SIZE 100
#define PORT 8083

char buffer[MAX_SIZE];
char *message = "Hello Client from Server\n";
char *server;

int main(int argc, char const *argv[])
{
	
	int bind_status;
	int sockfd;
	int len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	sockfd = socket(AF_INET, SOCK_DGRAM,0);
	if(sockfd < 0)
	{
		perror("Can not create socket");
		exit(0);
	}

	memset(&server_address, 0, sizeof(server_address)); 
	memset(&client_address, 0, sizeof(client_address));
	memset(buffer,'\0',MAX_SIZE);

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_address.sin_port = htons(PORT);

	bind_status=bind(sockfd,(const struct sockaddr*)&server_address,sizeof(server_address));
	if(bind_status < 0)
	{
		perror("Bind is not working ");
		exit(0);
	}

	int n;
	len = sizeof(client_address);

	while(1)
	{
		bzero(buffer, sizeof(buffer));
		n = recvfrom(sockfd,(char*)buffer,1024,MSG_WAITALL,(struct sockaddr*)&client_address,&len);
		buffer[n]='\0';
		//printf("Client>> %s\n", buffer);
		
		sendto(sockfd,(const char*)message,strlen(message),MSG_CONFIRM,(const struct sockaddr*)&client_address,len);
		//printf("Message sent to the client\n");

	}

	return 0;
}