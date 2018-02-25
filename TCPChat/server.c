#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>

#define MAXSIZE 100

main()
{
	int sockfd,newsockfd,retval;
	socklen_t actuallen;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr,clientaddr;
	char buff[MAXSIZE];
	char buff2[MAXSIZE];
	int a=0;
	int status;
	
	//Socket creation
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1)
	{
		printf("\nSocket creation error");
	}
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(3388);
	serveraddr.sin_addr.s_addr=htons(INADDR_ANY);
	
	//Binding socket
	retval=bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(retval==-1)
	{
		printf("Binding error");
		close(sockfd);
	}
	
	//Listening 
	retval=listen(sockfd,1);
	if(retval==-1)
	{
		close(sockfd);
	}

	actuallen=sizeof(clientaddr);

	//Accepting connection
	newsockfd=accept(sockfd,(struct sockaddr*)&clientaddr,&actuallen);
	if(newsockfd==-1)
	{
		close(sockfd);
	}
	
	int pid;
	pid = fork();

	if (pid == 0) {
		while(1) {
			//Sending messages
			gets(buff2);
			sentbytes=send(newsockfd,buff2,sizeof(buff2),0);
			if(sentbytes==-1 || (strcmp(buff2,"BYE") == 0))
			{
				printf("Stopped Sending\n");
				break;
			}
		}
		
	}

	else {
		while(1) {
			//Receiving message
			recedbytes=recv(newsockfd,buff,sizeof(buff),0);
			if(recedbytes==-1 || (strcmp(buff,"BYE") == 0))
			{
				printf("Stopped Receiving\n");
				break;
			}
			printf("Message Received: %s\n", buff);
		}
	}

	if(pid != 0) {
		wait(&status);
		close(sockfd);
		close(newsockfd);
	}
	
}

