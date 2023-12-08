#pragma once
#ifndef TCP_SERVER_H
#define TCP_SERVER_H
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <fstream>
#include "ThreadManager.h"

#pragma comment(lib, "Ws2_32.lib")

class Server {
private:
	WSADATA wsdata;
	SOCKET serverSocket;
public:
	Server(int port);
	~Server();

	void Run();

};

#endif // !TCP_SERVER_H
