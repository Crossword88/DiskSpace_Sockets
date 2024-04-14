#pragma once

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib,"winmm.lib") 
#include <winsock2.h>
#include <WS2tcpip.h>
#include <iostream>

#include <windows.h>
#include <stdio.h>
#include <vector>

#define port 1111
#define LocalHost "127.0.0.1"

struct diskData
{
	ULONGLONG usingSpace;
	ULONGLONG freeSpace;
	ULONGLONG totalSpace;

	diskData(ULONGLONG usingSpace, ULONGLONG freeSpace, ULONGLONG totalSpace)
	{
		this->usingSpace = usingSpace;
		this->freeSpace = freeSpace;
		this->totalSpace = totalSpace;
	}
};

bool WsaInitialization()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		std::cout << "Error WSA Startup\n";
		return false;
	}
	return true;
}

bool SocketCreateUDP(SOCKET* SockConnectionUDP)
{
	// Create UDP socket
	*SockConnectionUDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (*SockConnectionUDP == INVALID_SOCKET) {
		std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return false;
	}
	return true;
}

bool ClientBindingSocketUDP(SOCKET* SockConnectionUDP, SOCKADDR_IN* SockAddr)
{
	// Bind the socket to Send
	in_addr addr;
	if (inet_pton(AF_INET, LocalHost, &addr) <= 0)
	{
		std::cout << "Error to convert IPv4 address\n";
		return false;
	}

	SockAddr->sin_family = AF_INET;
	SockAddr->sin_port = htons(port); // Example port number
	SockAddr->sin_addr = addr; // Example server IP address

	return true;
}

bool SendDriveLetter(SOCKET SockConnectUDP, SOCKADDR_IN& SockAddr)
{
	char diskLetter;
	std::cout << "Information of wich disk you'd like to see?\n";
	std::cin >> diskLetter;
	int sended = sendto(SockConnectUDP, &diskLetter, sizeof(char), 0, (sockaddr*)&SockAddr, sizeof(SockAddr));
	if (sended != sizeof(char))
	{
		std::cout << "Data has sended not correctly\n";
		return false;
	}
	return true;
}

diskData ReceiveDriveData(SOCKET SockConnectionUDP, sockaddr_in SockAddr, int SockAddrSize)
{
	diskData data(0, 0, 0);
	recvfrom(SockConnectionUDP, reinterpret_cast<char*>(&data), sizeof(diskData), 0, (sockaddr*)&SockAddr, &SockAddrSize);
	return data;
}

void ShowDiskSpace(diskData diskInfo)
{
	std::cout << "Total disk space(Mb): " << diskInfo.totalSpace / 1024 / 1024 << std::endl;
	std::cout << "Free disk space(Mb): " << diskInfo.freeSpace / 1024 / 1024 << std::endl;
	std::cout << "Using disk space(Mb): " << diskInfo.usingSpace / 1024 / 1024 << "\n\n";
}