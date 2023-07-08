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
#include <net/if.h>
#include <ifaddrs.h>


int Broadcast_Start(int port);
//extern char* recvStrData(char* str);
