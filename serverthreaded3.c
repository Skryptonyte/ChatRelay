#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linkedlists.h>


#ifdef unix
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif


#ifdef _WIN32
#include <winsock2.h>
#endif

#include <pthread.h>

struct args{
char* message;
struct sockaddr_in client;
struct linkedlist* userlist;
};
void* clientSendBack(void* newptr){

puts("New thread created");
char tokens[3][256];
struct args newargs = *((struct args* ) newptr);
char* message = newargs.message;
struct sockaddr_in client = newargs.client;
char* token = strtok(message,":");

struct linkedlist* userlist = newargs.userlist;
int count = 0;
while (token != NULL){
strcpy(tokens[count],token);
token = strtok(NULL,":");
count++;
}

printf("Splitting into tokens- Message: %s, Port: %s, Username: %s\n",tokens[0],tokens[1],tokens[2]);

if (!search(userlist,tokens[2])){
struct linkedlist* ptr = push(userlist,tokens[2]);
ptr->port = htons(strtol(tokens[1],NULL,10));
ptr->ip = strdup(inet_ntoa(client.sin_addr));
}

struct linkedlist* ptr = userlist->next;
while (ptr){

char sendbuffer[256];
memset(sendbuffer,0,256);
strcpy(sendbuffer,tokens[0]);
strcat(sendbuffer,":");
strcat(sendbuffer,tokens[2]);
puts("Sending back");
struct sockaddr_in recclient;
recclient.sin_port = ptr->port;
recclient.sin_addr.s_addr = inet_addr(ptr->ip);
recclient.sin_family = AF_INET;
int sendbackfd = socket(AF_INET,SOCK_STREAM,0);
int connectresult = connect(sendbackfd,&recclient,sizeof(recclient));
if (connectresult >= 0)
{
send(sendbackfd,sendbuffer,strlen(sendbuffer),0);
close(sendbackfd);
ptr = ptr->next;
}
else{
exterminate(userlist,ptr->name);
ptr = ptr->next;
puts("Failed to send message");
close(sendbackfd);
}
}
return NULL;
}


int main(){
int sockfd = socket(AF_INET,SOCK_STREAM,0);
//struct clients* list = (struct clients* ) malloc(sizeof(struct clients)*100);
struct linkedlist* userlist = initlist("start");

struct sockaddr_in server;
server.sin_family = AF_INET;
server.sin_port = htons(4510);
server.sin_addr.s_addr = inet_addr("0.0.0.0");

setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int));

bind(sockfd,(struct sockaddr* ) &server,sizeof(server));
listen(sockfd,30);

while (1){
struct sockaddr_in client;
int size = sizeof(client);
char message[1024];

memset(message,0,1024);
puts("Message: ready");
int peerfd = accept(sockfd,(struct sockaddr* )&client,&size);

recv(peerfd,message,1024,0);
printf("Recieved packet: %s\n",message);

struct args newarg;
newarg.message = strdup(message);
newarg.client = client;
newarg.userlist = userlist;
pthread_t pid;
pthread_create(&pid,NULL,clientSendBack,&newarg);
//printf("Recieved packet: %s\n",message);

/*
printf("Splitting into tokens- Message: %s, Port: %s, Username: %s\n",tokens[0],tokens[1],tokens[2]);
if (!search(userlist,tokens[2])){
struct linkedlist* ptr = push(userlist,tokens[2]);
ptr->port = htons(strtol(tokens[1],NULL,10));
ptr->ip = strdup(inet_ntoa(client.sin_addr));
}

struct linkedlist* ptr = userlist->next;
while (ptr){
char sendbuffer[256];
memset(sendbuffer,0,256);
strcpy(sendbuffer,tokens[0]);
strcat(sendbuffer,":");
strcat(sendbuffer,tokens[2]);
puts("Sending back");
struct sockaddr_in recclient;
recclient.sin_port = ptr->port;
recclient.sin_addr.s_addr = inet_addr(ptr->ip);
recclient.sin_family = AF_INET;
int sendbackfd = socket(AF_INET,SOCK_STREAM,0);
int connectresult = connect(sendbackfd,&recclient,sizeof(recclient));
if (connectresult >= 0)
{
send(sendbackfd,sendbuffer,strlen(sendbuffer),0);
close(sendbackfd);
ptr = ptr->next;
}
else{
exterminate(userlist,ptr->name);
ptr = ptr->next;
puts("Failed to send message");
close(sendbackfd);
}
}
*/
iterate(userlist);
}
}

