#include "TCPServer.h"

inline std::string get_code(std::string path) {

	std::fstream file(path);

	if (!file.is_open()) {

		perror("Can't open file");

		return "";
	}

	std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	if (contents.empty())

		return "";

	else

		return contents;
}

void HandleClient(SOCKET clientSocket) {

	std::string htmlContent = get_code("index.html");

	std::string responseType = "HTTP/1.1 200 OK\r\n";

	std::string contentType = "Content-Type: text/html\r\n";

	if (htmlContent.empty())

		responseType = "HTTP/1.1 404 Not Found\r\n";

	std::string response = responseType + contentType + "\r\n" + htmlContent;

	send(clientSocket, response.c_str(), static_cast<int>(response.size()), 0);

	closesocket(clientSocket);
}


Server::Server(int port): serverSocket(INVALID_SOCKET) {
	
	if (WSAStartup(MAKEWORD(2, 2), &this->wsdata) != 0) {
		std::cerr << "Can't initilize wsa\n";
		return;
	}

	this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (this->serverSocket == INVALID_SOCKET) {
		std::cerr << "Can't create socket \n";
	}

	sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;

	inet_pton(AF_INET, "192.168.1.6", &serverAddr.sin_addr);

	serverAddr.sin_port = htons(port);

	if (bind(this->serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {

		std::cerr << "Can't bind server on port " << port << '\n';

		return;
	}

	if (listen(this->serverSocket, SOMAXCONN) == SOCKET_ERROR) {

		std::cerr << "Can't listen on port " << port << '\n';

		return;
	}

	std::cout << "Listening on port " << port << '\n';

}

Server::~Server() {

	closesocket(this->serverSocket);

	WSACleanup();

}

void Server::Run() {

	ThreadPool threadPool(2);

	while (true) {

		SOCKET clientSocket = accept(this->serverSocket, nullptr, nullptr);

		if (clientSocket == INVALID_SOCKET) {

			std::cerr << "Accept error \n";

			return;
		}

		sockaddr_in* addrClient = new sockaddr_in;

		if (addrClient == nullptr) {

			std::cerr << "Error in allocation memory \n";

		}

		int* addLen = new int; 

		*addLen = sizeof(sockaddr_in);

		if (getpeername(clientSocket, reinterpret_cast<sockaddr*>(addrClient), addLen) != 0) {

			std::cerr << "Error " << WSAGetLastError() << '\n';

		}

		char* clientIp = new char[INET_ADDRSTRLEN]{};

		if (inet_ntop(AF_INET, &addrClient->sin_addr, clientIp, INET_ADDRSTRLEN) == "") {

			std::cerr << "Error " << WSAGetLastError() << '\n';
		}

		std::cout << clientIp << " connected\n";

		delete addrClient;

		delete addLen;

		delete[] clientIp;

		// Create a new thread to handle the client
		threadPool.Enqueue([clientSocket]() { HandleClient(clientSocket); });

	}

}