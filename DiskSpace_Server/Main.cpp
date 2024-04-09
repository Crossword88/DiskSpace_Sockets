#include "Including.h"

int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 0;

	sockaddr_in SockAddr;
	SOCKADDR_IN clientInfo;
	SockAddr.sin_port = htons(port);
	SockAddr.sin_family = AF_INET;
	in_addr addr;
	int addrSize = sizeof(SockAddr);
	if (inet_pton(AF_INET, "127.0.0.1", &addr) <= 0)
	{
		return 0;
	}
	SockAddr.sin_addr = addr;


	SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0);

	if (bind(sListen, (SOCKADDR*)&SockAddr, addrSize) != 0)
		return 0;

	if (listen(sListen, SOMAXCONN) != 0)
		return 0;

	SOCKET newConnection = 0;
	std::cout << "Waiting Connection\n";
	newConnection = accept(sListen, (SOCKADDR*)&clientInfo, &addrSize);

	if (newConnection == 0) return 0;

	system("cls");
	std::cout << "Connected\n";

	char disk;
	recv(newConnection, &disk, sizeof(char), 0);
	std::cout << disk << std::endl;

	std::string diskName = "";
	diskName += disk;
	diskName += ':';
	std::cout << diskName << std::endl;

	ULARGE_INTEGER x, y;
	std::wstring diskNameW(diskName.begin(), diskName.end());
	LPCWSTR WinDiskName = diskNameW.c_str();
	GetDiskFreeSpaceEx(WinDiskName, &x, &y, 0);	// x = FreeBytes, y = totalBytes
	
	std::cout << x.QuadPart /1024/1024<< std::endl;
	std::cout << y.QuadPart /1024/1024<< std::endl;
	std::cout << (y.QuadPart - x.QuadPart) /1024/1024<< std::endl;

	diskData diskInfo(y.QuadPart - x.QuadPart, x.QuadPart, y.QuadPart);

	send(newConnection, reinterpret_cast<char*>(&diskInfo), sizeof(diskData), 0);
}