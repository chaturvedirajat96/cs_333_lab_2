/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <cstdlib>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <fstream>

using namespace std;

void error(char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	 int sockfd, newsockfd, portno;
	 socklen_t clilen;
	 char buffer[256];
	 struct sockaddr_in serv_addr, cli_addr;
	 int n;
	 if (argc < 2) {
		 fprintf(stderr,"ERROR, no port provided\n");
		 exit(1);
	 }

	 /* create socket */

	 sockfd = socket(AF_INET, SOCK_STREAM, 0);
	 if (sockfd < 0) 
		error("ERROR opening socket");

	 /* fill in port number to listen on. IP address can be anything (INADDR_ANY) */

	 bzero( (char *) &serv_addr, sizeof(serv_addr));
	 portno = atoi(argv[1]);
	 serv_addr.sin_family = AF_INET;
	 serv_addr.sin_addr.s_addr = INADDR_ANY;
	 serv_addr.sin_port = htons(portno);

	 /* bind socket to this port number on this machine */

	 if (bind(sockfd, (struct sockaddr *) &serv_addr,
			  sizeof(serv_addr)) < 0) 
			  error("ERROR on binding");
	 
	 while(true)
	 {
		 /* listen for incoming connection requests */

		 listen(sockfd,5);
		 clilen = sizeof(cli_addr);

		 /* accept a new request, create a newsockfd */

		 newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		 if (newsockfd < 0) 
			  error("ERROR on accept");
		  int ret = fork();
		  if(ret==0)
		  {
			/* read message from client */

			 bzero(buffer,256);
			 n = read(newsockfd,buffer,255);
			 if (n < 0) error("ERROR reading from socket");
			 printf("Here is the message: %s\n",buffer);
			 
			 int i;
			 for(i=0;i<255;i++) if(buffer[i]==' ') break;
			 i++;
			 string filename = "";
			 for(;i<255;i++) {if(buffer[i]=='.') break; filename+=buffer[i];}
			 filename+=".txt";
			 ifstream file (filename, ios::in);
			 if(file<0) {error("File not found");}

			 int length = 255;
			 char * buffer2 = new char [length];

			 /* send reply to client */
			 while(file)
			 {
			 	file.read(buffer2,length);
			 	n = write(newsockfd,buffer2,255);
				if (n < 0) {error("ERROR writing to socket");break;}
			 }
			 close(newsockfd);
			 return 0;

			 //write_to_socket
			 //n = write(newsockfd,"I got your message",18);
			 //if (n < 0) error("ERROR writing to socket");
		  }
		  else
		  {
		  	int status;
		  	while(waitpid(-1,&status,WNOHANG));
		  }
	  }
	 return 0; 
}
