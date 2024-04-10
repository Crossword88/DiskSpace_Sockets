#include "Including.h"

int main()
{
	if (!WsaInitialization())
	{
		return -1;
	}

	sockaddr_in SockAddr;
	if (!SetupSocketAddress(&SockAddr))
	{
		return -1;
	}
	int addrSize = sizeof(SockAddr);

	SOCKET ServConnect;
	if (!SockConnecting(&ServConnect, SockAddr, addrSize))
	{
		return -1;
	}

	SendDriveLetter(ServConnect);

	diskData diskInfo(0,0,0);
	recv(ServConnect, reinterpret_cast<char*>(&diskInfo), sizeof(diskData), 0);

	ShowDiskSpace(diskInfo);
}