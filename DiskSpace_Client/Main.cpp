#include "Including.h"

int main()
{
	sockaddr_in SockAddr;
	SOCKET SockConnectionUDP;
	int SockAddrSize = sizeof(SockAddr);

	if (!WsaInitialization())	return -1;
	if (!SocketCreateUDP(&SockConnectionUDP))	return -1;
	if (!ClientBindingSocketUDP(&SockConnectionUDP, &SockAddr))	return -1;

	while (true)
	{
		if(!SendDriveLetter(SockConnectionUDP, SockAddr)) break;
		diskData diskInfo = ReceiveDriveData(SockConnectionUDP, SockAddr, SockAddrSize);

		ShowDiskSpace(diskInfo);
	}

	WSACleanup();
	return 0;
}