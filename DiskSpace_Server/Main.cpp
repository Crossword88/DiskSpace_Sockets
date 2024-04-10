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

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0);

	if (!BindingSocket(&sListen, &SockAddr, addrSize))
	{
		return -1;
	}

	SOCKET SocketConnection = SocketConnectingToClient(sListen, addrSize);

	std::string diskName = GetDirectoryNameFromSocket(SocketConnection);
	std::cout << diskName << std::endl;
	
	/*std::cout << x.QuadPart /1024/1024<< std::endl;
	std::cout << y.QuadPart /1024/1024<< std::endl;
	std::cout << (y.QuadPart - x.QuadPart) /1024/1024<< std::endl;*/

	diskData diskSpace = GetDiskSpace(diskName);

	int sendBytes = send(SocketConnection, reinterpret_cast<char*>(&diskSpace), sizeof(diskData), 0);
	if (sendBytes != sizeof(diskData))
	{
		std::cout << "Data has sended not correctly\n";
		return -1;
	}

	return 0;
}