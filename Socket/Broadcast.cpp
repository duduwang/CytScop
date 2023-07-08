#include "Broadcast.h"



int B_PORT = 1900;
const char* IP;


const char*  getIpStr()
{
	struct ifaddrs *myaddrs, *ifa;
	void *in_addr;
	char buf[64];

	if(getifaddrs(&myaddrs) != 0)
	{
		perror("getifaddrs");
		exit(1);
	}

	for (ifa = myaddrs; ifa != NULL; ifa = ifa->ifa_next)
	{
		if (ifa->ifa_addr == NULL)
			continue;
		if (!(ifa->ifa_flags & IFF_UP))
			continue;

		switch (ifa->ifa_addr->sa_family)
		{
			case AF_INET:
				{
					struct sockaddr_in *s4 = (struct sockaddr_in *)ifa->ifa_addr;
					in_addr = &s4->sin_addr;
					break;
				}

			case AF_INET6:
				{
					struct sockaddr_in6 *s6 = (struct sockaddr_in6 *)ifa->ifa_addr;
					in_addr = &s6->sin6_addr;
					break;
				}

			default:
				continue;
		}

		if (inet_ntop(ifa->ifa_addr->sa_family, in_addr, buf, sizeof(buf)))
		{
			printf("%s: %s\n", ifa->ifa_name, buf);
		}
	}

	freeifaddrs(myaddrs);
    return &buf[0];
}

void* asyncData(void* ptr)
{
	
	int sock = *(int *)ptr;  
	while(1){
		char buf[40];
		int slen;
		struct sockaddr_in recvAddr;
		int rAddrLen = sizeof(recvAddr);
		if ((slen = recvfrom(sock, buf, 40, 0, (struct sockaddr *)&recvAddr, (socklen_t *)&rAddrLen)) > 0)
		{
			char sbuf[25];
			sprintf(sbuf,"ip:%s port:%d\n",IP,B_PORT);
			sendto(sock, sbuf, 20, 0, (struct sockaddr *)&recvAddr, sizeof(recvAddr));
			break;
		}
	}
}
int Broadcast_Start(int port)
{
	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	B_PORT = port;
	int bl = 1;
	if(-1 == setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &bl, sizeof(bl)))
	{
		return -1;
	}
	struct sockaddr_in locAddr;
	locAddr.sin_family      = AF_INET;
	locAddr.sin_port        = htons(B_PORT);
	locAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (-1 == bind(sock, (struct sockaddr *)&locAddr, sizeof(locAddr)))
	{
		return 0;
	}
	IP = getIpStr();
	pthread_t thrd1;  
	pthread_create(&thrd1, NULL, asyncData, &sock);  
}

/*
int main()
{
	broadcast_start();
	printf("started");
}

*/
