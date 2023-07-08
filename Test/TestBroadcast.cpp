
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
	int port = 1900;  
	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	int bl = 1;
	if (-1 == setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &bl,sizeof(bl)))
	{
		return -1;
	}
	/*

	sockaddr_in locAddr;
	locAddr.sin_family      = AF_INET;
	locAddr.sin_port        = htons(port);
	//locAddr.sin_port        = htons(B_PORT);
	locAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (-1 == bind(sock, (sockaddr *)&locAddr, sizeof(locAddr)))
	{
		return -1;
	}
	*/
	while (1)
	{
		char sbuf[20] = "123456";
		sockaddr_in sendAddr;
		sendAddr.sin_family = AF_INET;
		sendAddr.sin_port   = htons(port);
		sendAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
		sendto(sock, sbuf, 20, 0, (sockaddr *)&sendAddr, sizeof(sendAddr));

		printf("sended end\n");
		while (1)
		{
			int slen;
			char buf[40];
			sockaddr_in recvAddr;
			int rAddrLen = sizeof(recvAddr);
			if ((slen = recvfrom(sock, buf, 40, 0, (sockaddr *)&recvAddr, (socklen_t *)&rAddrLen)) > 0)
			{
				printf("Recv Data From UDP Server %s\n",buf);
				break;
			}
			else
			{
				continue;
			}
		}
	}
}

