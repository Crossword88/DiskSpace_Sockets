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

bool SetupSocketAddress(SOCKADDR_IN* SockAddr)
{
	SockAddr->sin_port = htons(port);
	SockAddr->sin_family = AF_INET;
	in_addr addr;
	if (inet_pton(AF_INET, LocalHost, &addr) <= 0)
	{
		std::cout << "Error to convert IPv4 address\n";
		return false;
	}
	SockAddr->sin_addr = addr;
	return true;
}

bool SockConnecting(SOCKET *ServConnect, SOCKADDR_IN SockAddr, int addrSize)
{
	*ServConnect = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(*ServConnect, (SOCKADDR*)&SockAddr, addrSize) != 0)
	{
		std::cout << "Error conecting to Server\n";
		return false;
	}
	return true;
}

void SendDriveLetter(SOCKET ServConnect)
{
	char diskLetter;
	std::cout << "Information of wich disk you'd like to see?\n";
	std::cin >> diskLetter;
	send(ServConnect, &diskLetter, sizeof(char), 0);
}

void ShowDiskSpace(diskData diskInfo)
{
	std::cout << "\nTotal disk space(Mb): " << diskInfo.totalSpace / 1024 / 1024 << std::endl;
	std::cout << "Free disk space(Mb): " << diskInfo.freeSpace / 1024 / 1024 << std::endl;
	std::cout << "Using disk space(Mb): " << diskInfo.usingSpace / 1024 / 1024 << std::endl;
}