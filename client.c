//Arnab Nath helped me to write this code 

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#define PORT 5563
#define SIZE 100

int main(int argc, char * argv[])
{

		FILE *fp;
		struct sockaddr_in servaddr;
		char str[SIZE];
		int sfd,len;

		bzero((char *)&servaddr, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		servaddr.sin_port = htons(PORT);


		if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			perror("Can not create Socket");
			exit(0);
		}
		else
			printf("Socket is created\n");

		if((connect(sfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) < 0)
		{
			perror("Connect is not working");
			exit(0);
		}
		else
			printf("Connecting...\n");
	  
		/*	
		while(fgets(str, sizeof(str), stdin)) 
		{
			str[SIZE-1] = '\0';
			len = strlen(str) + 1;
			send(sfd, str, len, 0);
		}*/

///////////////////////////////////////////////////////////
		bzero(str, sizeof(str)); 
		printf("Enter the file name : ");
		
		fgets(str, sizeof(str), stdin);
		write(sfd, str, sizeof(str)); 
		bzero(str, sizeof(str)); 

		int t,h,w,intensity,T,counter,i;
	    char format[10];
	    FILE *imginv;
	    bzero(format,10);

	    imginv = fopen("inverted","w");
	    if(imginv == NULL)
	    {
	    	perror("Error. Can't write");
	    	exit(0);
	    }

	    read(sfd,format,10);
	    fprintf(imginv,"%s",format);

	    read(sfd,&w,sizeof(w));
	    fprintf(imginv,"%d\n",ntohl(w));

	    read(sfd,&h,sizeof(h));
	    fprintf(imginv,"%d ",ntohl(h));

	    read(sfd,&t,sizeof(t));
	    fprintf(imginv,"%d\n",ntohl(t));

	    counter=0;

	    for(i=0; i<ntohl(w)*ntohl(h); i++)
	    {  
	    	read(sfd,&t,sizeof(t));
	        counter++;
	        if(counter >= 50)
	        {
	            fprintf(imginv,"%d\n",ntohl(t));
	            counter=0;
	        }
	        else
	            fprintf(imginv,"%d ",ntohl(t));
	       
	    }   
	    fclose(imginv);
	    printf("\nFile Downloaded\n");

////////////////////////////////////////////////////////////

        imginv = fopen("inverted","r");

        if(imginv == NULL)
        {
        	perror("Client cant open file");
        	exit(0);
        }

        bzero(format,10);
        fgets(format, sizeof(format), imginv);
        write(sfd,format,10);

        fscanf(imginv, "%d ", &h);
        T= htonl(h);
        write(sfd,&T,sizeof(T));

        fscanf(imginv, "%d ", &w);

        T= htonl(w);
        write(sfd,&T,sizeof(T));

        fscanf(imginv, "%d ", &intensity);
        T = htonl(intensity);
        write(sfd,&T,sizeof(T));

        for(int i=0; i<w*h; i++)
        {
            fscanf(imginv, "%d ", &t);
            T = htonl(intensity-t);
            write(sfd,&T,sizeof(T));
        }
        fclose(imginv);
        printf("\nInverted Image File Sent\n");


/*
    while(1)
    { 
        bzero(str, sizeof(str)); 
        printf("Client : "); 
        fgets(str, sizeof(str), stdin);
        write(sfd, str, sizeof(str)); 
        bzero(str, sizeof(str)); 
        read(sfd, str, sizeof(str)); 
        printf("Server >> %s", str); 
    } 
	*/
}