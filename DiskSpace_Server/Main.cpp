#include "Including.h"

int main()
{
	if (!WsaInitialization())
	{
		return -1;
	}

	sockaddr_in SockAddr;
	int SockAddrSize = sizeof(SockAddr);
	SOCKET SockConnectionUDP;

	if (!SocketSetUpUDP(&SockConnectionUDP, &SockAddr)) return -1;

	while (true)
	{
		std::string diskName = GetDirectoryNameFromSocket(SockConnectionUDP, SockAddr);
		std::cout << diskName << std::endl;

		diskData diskSpace = GetDiskSpace(diskName);

		std::cout << diskSpace.freeSpace /1024/1024<< std::endl;
		std::cout << diskSpace.totalSpace /1024/1024<< std::endl;
		std::cout << diskSpace.usingSpace /1024/1024<< std::endl;

		if (!SendingDiskSpaceUDP(diskSpace, SockConnectionUDP, SockAddr)) break;
	}

	WSACleanup();
	return 0;
}