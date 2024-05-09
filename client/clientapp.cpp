#include "client.h"

// Client entrypoint
int main()
{
    try {
        // Create an instance of Client
        Client client("127.0.0.1", SERVER_PORT);

        // Setup a connection to the server
        client.setupConnection();

        // Start receiving for the server
        client.recvMessages();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    // Exit with return code 0
	return 0;
}