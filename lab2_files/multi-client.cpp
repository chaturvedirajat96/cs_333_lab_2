#include <stdio.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <iostream>

#define MAX_FILE_NO 10000;
void error(char *msg)
{
    perror(msg);
    exit(0);
}

struct client_attr
{
	struct sockaddr_in server_addr;
	float duration;
	float sleep_time;
	bool mode;
	
};

void *client(void *attr_)
{	
	client_attr *attr = (client_attr *)attr_;
	int sock_fd;
	//open socket
	     
  //Read and Write to Socket
  int b_size = 1024;
  char buffer[b_size];
  int num_b, diff_time, file_n;
  time_t start_t = time(NULL),curr_t = time(NULL);
  diff_time = (int) ((curr_t - start_t)*1000.0)/(CLOCKS_PER_SEC/1000);
  while(attr->duration > diff_time )
  {	
  	if((sock_fd = socket(AF_INET, SOCK_STREAM, 0) )< 0)
    pthread_exit((void *)-1);
    //connect to server
    if (connect(sock_fd,(struct sockaddr *)&attr->server_addr,sizeof(attr->server_addr)) < 0) 
    pthread_exit((void *)-1);
  
    if(attr->mode){file_n = rand()%MAX_FILE_NO;}
  	else file_n=0;
  	//Send Request
  	sprintf(buffer, "get files/foo%d.txt",file_n);
  	if( ( num_b = write(sock_fd, buffer, strlen(buffer)) ) < 0)
  		fprintf(stderr, "Error Writing to Socket");
    	bzero(buffer,b_size);

  	//Receive File
  	while((read(sock_fd, buffer, 1023)>0)){
			//Discard recieved data		
  	}

  	usleep(attr->sleep_time*1000);
  	std::cout<<"Next\n";
  	curr_t = time(NULL);
    diff_time = (int) ((curr_t - start_t)*1000.0)/(CLOCKS_PER_SEC/1000);
    std::cout<<diff_time<<"\n";
    close(sock_fd);
  }
  pthread_exit((void *)0);


}
/* fill in server address in sockaddr_in datastructure */

struct sockaddr_in set_server(struct sockaddr_in &server_addr, struct hostent *server, int &port_no)
{
	bzero((char *) &server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
  server_addr.sin_port = htons(port_no);
  return server_addr;
}    




int main(int argc, char *argv[])
{	
	//Arguments
	if (argc < 7) {
       fprintf(stderr,"usage %s <hostname> <port> <number of users> <duration> <Sleep Time> <mode>\n", argv[0]);
       exit(0);
    }
    client_attr attr;
    //processing arguments
    int num_of_threads = atoi(argv[3]);
    attr.duration = atoi(argv[4]);
    attr.sleep_time = atoi(argv[5]);
    attr.mode = (strcmp(argv[6],"fixed"));
	int port_no = atoi(argv[2]);
    
    //Server info
    struct hostent *server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    //setup server
    set_server(attr.server_addr, server, port_no);
   
   	//Create Threads
   	pthread_t threads[num_of_threads];
   	int error_th;
   	for(int i = 0;i<num_of_threads;i++)
   	{
   		if( (error_th=pthread_create(&threads[i], NULL, client, &attr)) < 0)
   			fprintf(stderr,"Error Creating Thread %d\n",i);
   	}

   	// free attribute and wait for the other threads
   	//Join threads
   	void *status;
   	int rthread;
   	for(int i = 0;i<num_of_threads;i++)
   	{
   		if( (rthread = pthread_join(threads[i], &status) < 0) )
   			fprintf(stderr,"Error Joining thread %d \n",i);
   	}

    return 0;
}
