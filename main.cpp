#include "TCPServer.h"

#pragma comment(lib, "Ws2_32.lib")

int main() {

    Server server(8080);

    server.Run();
    
    return 0;
}
