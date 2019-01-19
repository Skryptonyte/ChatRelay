#include <string.h>
#include <stdlib.h>
struct linkedlist{
char* name;
char* ip;
int port;
struct linkedlist* next;
};


struct linkedlist* initlist(){
struct linkedlist* newlist = malloc(sizeof(struct linkedlist));
newlist->name = "start";
newlist->next = NULL;
return newlist;
}
struct linkedlist* push(struct linkedlist* list, char* name){
struct linkedlist* listtouse = list;
while (listtouse->next){
listtouse = listtouse->next;
}
listtouse->next = malloc(sizeof(struct linkedlist));
listtouse->next->name = strdup(name);
listtouse->next->next = NULL;
printf("Pushed new element to list: %s\n",listtouse->next->name);
return listtouse->next;
}

void pop(struct linkedlist* list){
if (!list->next){
printf("You can't pop element at zero index\n");
return;
}
struct linkedlist* listtouse = list;
while (listtouse->next->next){
listtouse = listtouse->next;
}
printf("Popped element from list: %s\n",listtouse->next->name);
free(listtouse->next->name);
free(listtouse->next);
listtouse->next = NULL;
}

void iterate(struct linkedlist* list){
int count = 1;
struct linkedlist* ptr = list->next;
while (ptr){
printf("Element %d: %s\n",count,ptr->name);
ptr = ptr->next;
count++;
}
}

void exterminate(struct linkedlist* list, const char* name){
struct linkedlist* ptr = list->next;
struct linkedlist* prev = list;
while (ptr){
if (!strcmp(ptr->name,name)){
printf("Removed element: %s\n",name);
puts("Breaks from this point");
prev->next = ptr->next;
free(ptr->name);
free(ptr);
}
prev = ptr;
ptr = ptr->next;
}
}

void insert(struct linkedlist* list, int count,const char* name){
int countl = 0;
struct linkedlist* ptr = list;
struct linkedlist* prev = NULL;
while (ptr){
if (count == countl ){
struct linkedlist* newptr = malloc(sizeof(struct linkedlist));
prev->next = newptr;
newptr->next = ptr;
newptr->name = strdup(name);
}
prev = ptr;
ptr = ptr->next;
countl++;
}
}


int search(struct linkedlist* list, const char* name){
struct linkedlist* ptr = list->next;
while (ptr){
if (!strcmp(ptr->name,name)){
printf("Element found: %s\n",name);
return 1;
}
ptr = ptr->next;
}
return 0;
}
