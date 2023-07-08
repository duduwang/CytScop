#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>  
#include <errno.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h> 
#include <pthread.h>  


void* recvsocket(void *ptr)  ;
void* sendsocket(void *ptr)  ;
int socket_Init(int port);
int Socket_Start();
int getconnum();
void* socketSend(char* data,size_t len);
extern char* recvStrData(char* str);
