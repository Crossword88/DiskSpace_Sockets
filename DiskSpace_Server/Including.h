#pragma once

#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib,"winmm.lib") 
#include <winsock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>

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

bool SocketSetUpUDP(SOCKET* SockConnectionUDP, SOCKADDR_IN* SockAddr)
{
	// Create UDP socket
	*SockConnectionUDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (*SockConnectionUDP == INVALID_SOCKET) {
		std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return false;
	}

	// Client address
	memset((char*)SockAddr, 0, sizeof(SockAddr));
	SockAddr->sin_family = AF_INET;
	SockAddr->sin_port = htons(port); // Example port number
	SockAddr->sin_addr.s_addr = INADDR_ANY;
	if (bind(*SockConnectionUDP, (sockaddr*)SockAddr, sizeof(*SockAddr)) == SOCKET_ERROR) {
		std::cerr << "Binding failed with error: " << WSAGetLastError() << std::endl;
		closesocket(*SockConnectionUDP);
		WSACleanup();
		return false;
	}

	in_addr addr;
	if (inet_pton(AF_INET, LocalHost, &addr) <= 0)
		return false;

	return true;
}

std::string GetDirectoryNameFromSocket(SOCKET SockConnectUDP, SOCKADDR_IN& SockAddr)
{
	int SockAddrSize = sizeof(SockAddr);
	char disk;
	recvfrom(SockConnectUDP, &disk, sizeof(char), 0, (sockaddr*)&SockAddr, &SockAddrSize);

	std::string diskName = "";
	diskName += disk;
	diskName += ':';

	return diskName;
}

bool SendingDiskSpaceUDP(diskData diskSpace, SOCKET SockConnectUDP, SOCKADDR_IN SockAddr)
{
	int sendBytes = sendto(SockConnectUDP, reinterpret_cast<char*>(&diskSpace), sizeof(diskData), 0, (sockaddr*)&SockAddr, sizeof(SockAddr));
	if (sendBytes != sizeof(diskData))
	{
		std::cout << "Data has sended not correctly\n";
		return false;
	}
	return true;
}

diskData GetDiskSpace(std::string diskName)
{
	ULARGE_INTEGER freeBytes, totalBytes;
	std::wstring diskNameW(diskName.begin(), diskName.end());
	LPCWSTR WinDiskName = diskNameW.c_str();
	GetDiskFreeSpaceEx(WinDiskName, &freeBytes, &totalBytes, 0);

	diskData diskInfo(totalBytes.QuadPart - freeBytes.QuadPart, freeBytes.QuadPart, totalBytes.QuadPart);

	return diskInfo;
}