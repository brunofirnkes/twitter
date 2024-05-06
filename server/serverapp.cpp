#include "server.h"

// Server entrypoint
int main()
{
    try {
        // Create an instance of Server
        Server server;

        // Start listening to connections
        server.listenConnections();
    }
    catch(const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }

    // End
    std::cout << "Server stopped" << std::endl;
	return 0;
}