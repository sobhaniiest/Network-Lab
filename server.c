//Arnab Nath helped me to write this code 

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#define PORT 5563
#define SIZE 100


int main()
{
		struct sockaddr_in servaddr;
		char str[SIZE],buff[SIZE];
		FILE *imgreq,*imgnew;
		

		bzero((char *)&servaddr, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = INADDR_ANY;
		servaddr.sin_port = htons(PORT);

		int sfd,nsfd;

		if((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			perror("Can not create Socket");
			exit(0);
		}
		else
			printf("Socket is created\n");

		if((bind(sfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) < 0)
		{
			perror("Bind is not working");
			exit(0);
		}
		else
			printf("Socket is binded\n");

		listen(sfd, 1);

		int len = sizeof(servaddr);

		while(1)
		{
			if((nsfd = accept(sfd, (struct sockaddr *)&servaddr, &len)) < 0)
			{
				perror("Not accepting");
				exit(0);
			}
			else
				printf("accepting...\n");
			/*
			while(len = recv(nsfd, str, sizeof(str), 0))
				fputs(str, stdout);
			*/



    //////////////////////////////////////////////////////////////////
		bzero(str, SIZE); 
	    read(nsfd, str, sizeof(str));
	    printf("File requested : %s", str); 
	    int i,l,counter;
	    char new[SIZE];
	    l = strlen(str);
	    for(i=0;i<l-1;i++)
	    {
	    	buff[i] = str[i]; 
	    	new[i] = str[i];
	    }

	    strcat(new,"_new");
  
	    imgreq = fopen(buff,"r");
	    if(imgreq == NULL)
        {
        	perror("ERROR Server cant open the file");
        	exit(0);
        }
	    
	    char format[10];
	    int w,h,in,T,t;
	    fgets(format, sizeof(format), imgreq);
	    write(nsfd,format,10);

	    fscanf(imgreq, "%d ", &w);
	    T = htonl(w);
	    write(nsfd,&T,sizeof(T));

	    fscanf(imgreq, "%d ", &h);
	    T = htonl(h);
	    write(nsfd,&T,sizeof(T));

	    fscanf(imgreq, "%d ", &in);
	    T = htonl(in);
	    write(nsfd,&T,sizeof(T));

	    for(i=0; i<w*h; i++)
	    {
	        fscanf(imgreq, "%d ", &t);
	        T = htonl(t);
	        write(nsfd,&T,sizeof(T));
	    }

	    fclose(imgreq);
	    printf("\nFile sent\n");
	    bzero(format,10);

	///////////////////////////////////////////////////

		imgnew = fopen(new,"w");
	    if(imgnew == NULL)
	    {
	    	perror("Error. Can't write");
	    	exit(0);
	    }

	    read(nsfd,format,10);
	    fprintf(imgnew,"%s",format);

	    read(nsfd,&w,sizeof(w));
	    fprintf(imgnew,"%d ",ntohl(w));

	    read(nsfd,&h,sizeof(h));
	    fprintf(imgnew,"%d\n",ntohl(h));

	    read(nsfd,&t,sizeof(t));
	    fprintf(imgnew,"%d\n",ntohl(t));

	    counter=0;
	    for(i=0; i<ntohl(w)*ntohl(h); i++)
	    {  
	    	read(nsfd,&t,sizeof(t));
	        counter++;
	        if(counter>=50)
	        {
	            fprintf(imgnew,"%d\n",ntohl(t));
	            counter=0;
	        }
	        else
	            fprintf(imgnew,"%d ",ntohl(t));
	       
	    }   
	    fclose(imgnew);
	    printf("\nReceived file from client\n");
		    
	        /*

		    while(1)
		    { 
		        bzero(str, SIZE); 
		        read(nsfd, str, sizeof(str));  
		        printf("Client >> %s", str); 
		        printf("Server : "); 
		        bzero(str, SIZE); 
		        fgets(str, sizeof(str), stdin);
		        write(nsfd, str, sizeof(str));  
		    } 
		    */
			close(nsfd);
		}

}