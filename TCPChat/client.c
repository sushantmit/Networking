#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

#define MAXSIZE 100

main()
{
	int sockfd,retval;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr;
	char buff[MAXSIZE];
	char buff2[MAXSIZE];
	int status;

	//Socket creation
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd==-1)
	{
		printf("\nSocket Creation Error");
	}
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(3388);
	serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	//Connecting
	retval=connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
	if(retval==-1)
	{
		printf("Connection error");
	}
	
	int pid;
	pid = fork();

	if(pid == 0) {
		while(1) {
			//Sending Message
			gets(buff);
			sentbytes=send(sockfd,buff,sizeof(buff),0);
			if(sentbytes==-1 || (strcmp(buff,"BYE") == 0))
			{
				printf("Stopped Sending\n");
				break;
			}
		}
	}

	else {
		while(1) {
			//Receiving Message
			recedbytes=recv(sockfd,buff2,sizeof(buff2),0);
			if(recedbytes==-1 || (strcmp(buff2,"BYE") == 0))
			{
				printf("Stopped Receiving\n");
				break;
			}
			printf("Message Received: %s\n", buff2);
		}
	}
	
	if(pid!=0) {
		wait(&status);
		close(sockfd);
	}
}
