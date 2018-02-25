#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#define max 50

int main()
{
	int sockfd,newsockfd,k;
	socklen_t actuallen;
	int retval;
	size_t leng;
	char c;
	int recedbytes,sentbytes;
	struct sockaddr_in serveraddr,clientaddr;
	char buff[max],buff2[max];
	int status;
	
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd==-1)
	{
		printf("Socket creation error\n");
		exit(0);
	}
	
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(3391);
	serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	clientaddr.sin_family=AF_INET;
	clientaddr.sin_port=htons(3392);
	clientaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	
	retval=bind(sockfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));
	if(retval==-1)
	{
		printf("Binding error\n");
		close(sockfd);
	 	exit(0);
	}
	actuallen=sizeof(clientaddr);

	int pid;
	pid = fork();

	if(pid != 0) {
		while(1) {
			retval=recvfrom(sockfd,buff2,sizeof(buff2),0,(struct sockaddr *)&clientaddr,&actuallen);
			if(retval==-1 || (strcmp(buff2, "BYE") == 0))
			{
				printf("Stopped Receiving\n");
				break;
			}
			printf("Message received: %s\n", buff2);
		}
	}
	
	else {
		while(1) {
			gets(buff);
			actuallen=sizeof(clientaddr);
			retval=sendto(sockfd,buff,sizeof(buff),0,(struct sockaddr *)&clientaddr,actuallen);
			if(retval==-1 || (strcmp(buff, "BYE") == 0))
			{
				printf("Stopped Sending\n");
				break;
			}
		}
	}

	if(pid != 0) {
		wait(&status);
		close(sockfd);
	}
}

