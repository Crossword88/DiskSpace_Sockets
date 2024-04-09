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