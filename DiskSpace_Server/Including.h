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

std::string GetDirectoryNameFromSocket(SOCKET newConnection)
{
	char disk;
	recv(newConnection, &disk, sizeof(char), 0);

	std::string diskName = "";
	diskName += disk;
	diskName += ':';

	return diskName;
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

bool BindingSocket(SOCKET *sListen, SOCKADDR_IN *SockAddr, int addrSize)
{
	if (bind(*sListen, (SOCKADDR*)SockAddr, addrSize) != 0)
	{
		std::cout << "Error to bind address to socket\n";
		return false;
	}

	if (listen(*sListen, SOMAXCONN) != 0)
	{
		std::cout << "Error to listen socket\n";
		return true;
	}

	return true;
}

SOCKET SocketConnectingToClient(SOCKET sListen, int addrSize)
{
	SOCKET SockConnection = 0;
	sockaddr_in clientInfo;
	std::cout << "Waiting Connection\n";
	SockConnection = accept(sListen, (SOCKADDR*)&clientInfo, &addrSize);

	if (SockConnection == 0)
	{
		std::cout << "Error connecting to server\n";
		return 0;
	}

	system("cls");
	std::cout << "Connected\n";

	return SockConnection;
}