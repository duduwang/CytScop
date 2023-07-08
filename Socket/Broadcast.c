#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h> 


int B_PORT = 1900;

int broadcast_start()
{
	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	int bl = 1;
	if(-1 == setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &bl, (socklen_t *)sizeof(bl)))
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
	while(1){
		char buf[40];
		int slen;
		struct sockaddr_in recvAddr;
		int rAddrLen = sizeof(recvAddr);
		if ((slen = recvfrom(sock, buf, 40, 0, (struct sockaddr *)&recvAddr, (socklen_t *)&rAddrLen)) > 0)
		{
			char sbuf[40]="123123123123";
			printf("Recv Data From UDP query  %s\n",buf);
			sendto(sock, sbuf, 20, 0, (struct sockaddr *)&recvAddr, sizeof(recvAddr));
			break;
		}
	}
}
int main()
{
	broadcast_start();
	printf("started");
}


