//Standard libraries
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

//Libraries specific to Linux/OS X/FreeBSD/Other UNIXes
#ifdef unix
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

//Libraries specific to Windows
#ifdef _win32
#include <winsock2.h>
#endif

// Requires additional header file to compile on Windows
#include <pthread.h>

struct args{
int port;
char* name;
} a1;


void *listener(void* ptr){

struct args* arguments = (struct args* ) ptr;

puts("Starting listening thread..");
struct sockaddr_in listener;
int sockfd = socket(AF_INET,SOCK_STREAM,0);

listener.sin_family = AF_INET;
listener.sin_port = htons(arguments->port);
listener.sin_addr.s_addr = inet_addr("0.0.0.0");

bind(sockfd,(struct sockaddr* ) &listener,sizeof(listener));
listen(sockfd,5);

while (1){
puts("");
char tokens[3][256];
char buffer[256];
memset(tokens,0,sizeof(tokens));
memset(buffer,0,sizeof(tokens));

struct sockaddr_in clientaddr;
int socklen = sizeof(clientaddr);

int peerfd = accept(sockfd,(struct sockaddr* )&clientaddr,&socklen);
recv(peerfd,buffer,256,0);

char* token = strtok(buffer,":");
int i = 0;
while (i<=2 && token != NULL){
strcpy(tokens[i],token);
token = strtok(NULL,":");
i++;
}

printf("%s: %s",tokens[1],tokens[0]);
//fflush(stdout);
close(peerfd);
}
return NULL;
}


int main(int argc, char* argv[]){
pthread_t pid;
puts("Obtaining port..");
a1.port = strtol(argv[1],NULL,10);
puts("Obtaining username");
a1.name = argv[2];
pthread_create(&pid,NULL,listener,(void* )&a1);
puts("== GPSRelay v0.1.0 Beta ==");
start:
while (1){
struct sockaddr_in server;

server.sin_family = AF_INET;
server.sin_port = htons(4510);
server.sin_addr.s_addr = inet_addr("127.0.0.1");

char buffer[256];
memset(buffer,0,256);

int sockfd = socket(AF_INET,SOCK_STREAM,0);

fgets(buffer,256,stdin);

for (int i = 0; i<256; i++){
if (buffer[i] == '\n'){
buffer[i] = 0;
}
}

if (buffer[0] == '/'){
if (strcmp(buffer+1,"quit") == 0){
	puts("Quitting..");
	exit(0);
}
else if (strcmp(buffer+1,"help") == 0){
        puts("Placeholder for future use");
	goto start;
}
}

strcat(buffer,":");
strcat(buffer,argv[1]);
strcat(buffer,":");
strcat(buffer,argv[2]);

int connectresult = connect(sockfd,(struct sockaddr* )&server,sizeof(server));

if (connectresult == 0){
send(sockfd,buffer,strlen(buffer),0);
}
else{
printf("Socket connection error: %d\n",errno);
}
}
}
