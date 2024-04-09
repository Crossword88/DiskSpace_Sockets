#include "Including.h"

int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 0;

	sockaddr_in SockAddr;
	SockAddr.sin_port = htons(port);
	SockAddr.sin_family = AF_INET;
	in_addr addr;
	int addrSize = sizeof(SockAddr);
	if (inet_pton(AF_INET, "127.0.0.1", &addr) <= 0)
	{
		return 0;
	}
	SockAddr.sin_addr = addr;

	SOCKET ServConnect = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(ServConnect, (SOCKADDR*)&SockAddr, addrSize) != 0)
		return 0;

	char diskLetter;
	std::cout << "Information of wich disk you'd like to see?\n";
	std::cin >> diskLetter;
	send(ServConnect, &diskLetter, sizeof(char), 0);

	diskData diskInfo(0,0,0);
	recv(ServConnect, reinterpret_cast<char*>(&diskInfo), sizeof(diskData), 0);

	std::cout <<"\nTotal disk space(Mb): " << diskInfo.totalSpace/1024/1024 << std::endl;
	std::cout <<"Free disk space(Mb): " << diskInfo.freeSpace/1024/1024 << std::endl;
	std::cout <<"Using disk space(Mb): " << diskInfo.usingSpace/1024/1024 << std::endl;
}







