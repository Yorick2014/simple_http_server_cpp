#include <iostream>

#ifdef __linux__
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <unistd.h>
#elif _WIN32
	#include <winsock2.h>
	#include <WS2tcpip.h>
	#pragma comment(lib, "ws2_32.lib")
#endif


int main()
{
	#ifdef _WIN32
	// Initialize Winsock
	WSADATA wsaData;
	if (WSAStartup (MAKEWORD(2,2), &wsaData) != 0){
		std::cerr << "WSAStartup failed " << WSAGetLastError() << std::endl;
		return 1;
	}

	//----------------------
    // Create a SOCKET for listening for
    // incoming connection requests.
	SOCKET ListenSocket;
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket = INVALID_SOCKET){
		std::cerr << "ListenSocket failed with error: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}

	//----------------------
    // The sockaddr_in structure specifies the address family,
    // IP address, and port for the socket that is being bound.
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_port = htons(27015);
    inet_pton(AF_INET, "127.0.0.1", &service.sin_addr);

    if (bind(ListenSocket,
             (SOCKADDR *) & service, sizeof (service)) == SOCKET_ERROR) {
		std::cerr << "bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

	//----------------------
    // Listen for incoming connection requests.
    // on the created socket
    if (listen(ListenSocket, 1) == SOCKET_ERROR) {
		std::cerr << "listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

	//----------------------
    // Create a SOCKET for accepting incoming requests.
    SOCKET AcceptSocket;
    wprintf(L"Waiting for client to connect...\n");

    //----------------------
    // Accept the connection.
    AcceptSocket = accept(ListenSocket, NULL, NULL);
    if (AcceptSocket == INVALID_SOCKET) {
		std::cerr << "accept failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    } else
		std::cerr << "Client connected." << std::endl;

    // No longer need server socket
    closesocket(ListenSocket);
	#elif __linux__
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

	int opt = 1;
	int srv = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    if (srv != 0) {
        std::cerr << "Error setting socket options" << std::endl;
        return 1;
    }
	#endif
	
	#ifdef __linux__

	#elif _WIN32
	#endif

	#ifdef _WIN32
	WSACleanup();
	#elif __linux__
	#endif
	return 0;
}