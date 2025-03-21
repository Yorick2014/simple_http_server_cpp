#include <iostream>

#ifdef __linux__
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <unistd.h>
#elif _WIN32
	#include <winsock2.h>
	#pragma comment(lib, "ws2_32.lib")
#endif


int main()
{
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

	int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "Error setting socket options" << std::endl;
        return 1;
    }

	return 0;
}