#include"Socket.h" 

int client_st = 0;  
void* recvsocket(void *ptr)  
{  
	int fd = *(int *)ptr;  
	
	char *dest=NULL;
	char *tmp=NULL ;
	char *p = NULL;
	int tmpSize = 1024;
	tmp = (char*)malloc(sizeof(char)*tmpSize);
	memset(tmp, '\0', tmpSize);  
	while (int numbytes = recv(fd, tmp, tmpSize, 0))  
	{  
		if (numbytes == 0) 
		{ 
			printf("client quit!!\n");
			break;  
		}else if(numbytes > 0){
			int size = 0;
			int nbytes = numbytes;
			do{
				size = size +nbytes;
				if( nbytes  == tmpSize )
				{
					if(size != nbytes)
					{
						p = dest;
						dest  = (char*)malloc((size*2)*sizeof(char));
						memset(dest, '\0', size*2);
						memcpy(dest,p,size);
					}else{
						dest  = (char*)malloc((size*2)*sizeof(char));
						memset(dest, '\0', size*2);
					}
					dest = strcat(dest,tmp);
					if(p != NULL){
						free(p);
						p=NULL;
					}
				}else{
					p = dest;
					dest = (char*)malloc((size+1)*sizeof(char));
					memset(dest, '\0', size+1);
					if(p != NULL)
					{
						memcpy(dest,p,size - nbytes);
						dest = strcat(dest,tmp);
						recvStrData(dest);
						free(p);
						p=NULL;
						free(dest);
						dest =NULL;
					}else{
						memcpy(dest,tmp, nbytes);
						char* response = recvStrData(dest);
						socketSend(response,strlen(response));
						free(response);
						response = NULL;
						free(dest);
						dest = NULL;
					}
					memset(tmp, '\0', tmpSize);
					break;
				}
				size = nbytes+size;
				memset(tmp, '\0', tmpSize);
			}while((nbytes = recv(fd, tmp, tmpSize, MSG_DONTWAIT)) > 0);
		}
	}  
	printf("connection is  closed\n");  
	close(fd);
	return NULL;  
}  

void* sendsocket(void *ptr)  
{  
	int fd = *(int *)ptr;  
	char str[1024];  

	while (1)  
	{  
		memset(str, 0, sizeof(str));  
		read(STDIN_FILENO, str, sizeof(str));  
		send(fd, str, strlen(str), 0);  
	}  

	return NULL;  
}


void* socketSend(char* data,size_t len)
{

	int strlen = 1024;
	char str[strlen];
	int sendLen =0;
//printf("%d\n",len) ;
	if(len <= sizeof(str)){
		send(client_st,data,len,0);
	}else{
		do{

			if(sendLen >=len){
				printf("sendLen:%d   len: :%lu",sendLen,len);
				break;
			}


			memset(str,0,strlen);
			if((len -sendLen)> sizeof(str) ){

				memcpy(str,data+sendLen,strlen);
				usleep(1000);

				send(client_st,str,strlen,0);
				sendLen += strlen;
			}else{
				memcpy(str,data+sendLen, len-sendLen);
				send(client_st,str,len-sendLen,0);
				sendLen += len-sendLen;
			}
		}while(1);



	}    

}


int st;
int connum =0;
int socket_Init()
{

	int port = 1234;  
	st = socket(AF_INET, SOCK_STREAM, 0); 

	int opt = SO_REUSEADDR;
	setsockopt(st, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in addr; 
	memset(&addr, 0, sizeof(addr));  
	addr.sin_family = AF_INET; 
	addr.sin_port = htons(port);  
	addr.sin_addr.s_addr = htonl(INADDR_ANY); 


	if (bind(st, (struct sockaddr *) &addr, sizeof(addr)) == -1)  
	{  
		printf("bind failed %s\n", strerror(errno));  
		return EXIT_FAILURE;  
	}  


	if (listen(st, 20) == -1)  
	{  
		printf("listen failed %s\n", strerror(errno));  
		return EXIT_FAILURE;  
	}  
	printf("listen success\n");  
}


int Socket_Start()
{

	struct sockaddr_in client_addr;

	pthread_t thrd1, thrd2;  
	while (1)  
	{  
		memset(&client_addr, 0, sizeof(client_addr));  
		socklen_t len = sizeof(client_addr);  
		printf("waiting for client.......\n");
		client_st = accept4(st, (struct sockaddr*) &client_addr, &len,SOCK_NONBLOCK);  
		if (client_st == -1)  
		{  
			printf("accept failed %s\n", strerror(errno));  
			return EXIT_FAILURE;  
		} 
		connum ++;
		printf("accept by %s\n", inet_ntoa(client_addr.sin_addr));  
		pthread_create(&thrd1, NULL, recvsocket, &client_st);  
		// pthread_create(&thrd2, NULL, sendsocket, &client_st);  
	}  
	close(st);  

}


int getconnum()
{
	return connum;

}
