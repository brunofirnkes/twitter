#include "server.h"

// Static variables
std::atomic<bool> Server::stop_issued;
std::map<int, pthread_t> Server::connection_handler_threads;
int Server::_socket; 

Server::Server()
{
    manager = new Manager;
    _socket = -1;
    setupConnection();
}

Server::~Server()
{
    close(_socket);
    delete manager;
}

void Server::listenConnections()
{
    int client_socket = -1;

    // Set passive listen socket
    if (listen(_socket, 3) < 0)
        throw std::runtime_error("Error setting socket as passive listener");

    // Spawn thread for listening to administrator commands
    pthread_create(&command_handler_thread, NULL, handleCommands, NULL);

    // Admin instructions
    std::cout << "Server is ready to receive connections" << std::endl;

    // Wait for incoming connections
    int sockaddr_size = sizeof(struct sockaddr_in);
    int* new_socket;
    while(!stop_issued && (client_socket = accept(_socket, (struct sockaddr*)&client_address, (socklen_t*)&sockaddr_size)))
    {
        std::cout << "New connection accepted from client " << client_socket << std::endl;

        // Start new thread for new connection
        pthread_t comm_thread;

        // Get reference to client socket
        new_socket = (int*)malloc(sizeof(int));
        *new_socket = client_socket;

        // Spawn new thread for handling that client
        if (pthread_create(&comm_thread, NULL, handleConnection, (void*)new_socket) < 0) {
            // Close socket if no thread was created
            std::cerr << "Could not create thread for socket " << client_socket << std::endl;
            close(client_socket);
        }

        // Add thread to list of connection handlers
        Server::connection_handler_threads.insert(std::make_pair(*new_socket, comm_thread));
    }

    std::cout << "End server loop" << std::endl;

    // Wait for all threads to finish
    for (std::map<int, pthread_t>::iterator i = Server::connection_handler_threads.begin(); i != Server::connection_handler_threads.end(); ++i)
    {
        std::cout << "Waiting for client communication to end..." << std::endl;
        pthread_t* ref = &(i->second);
        pthread_join(*ref, NULL);
    }
}

void *Server::handleCommands(void* arg)
{
    // Get administrator commands
    std::string command;
    while(std::getline(std::cin, command)) {
        // TODO: Improve commands
        if (command == "exit")
            break;
    }

    // Signal all other threads to end
    Server::stop_issued = true;

    pthread_exit(NULL);
}

void *Server::handleConnection(void* arg)
{
    int socket = *(int*)arg;    // Client socket
    int readBytes = -1;         // Number of bytes read from the message
    char buffer[2048];   // Buffer for client message, maximum of BUFFER_SIZE bytes

    while((readBytes = recv(socket, buffer, BUFFER_SIZE, 0)) > 0) {
        // Decode received data into a packet structure
        packet* receivedPacket = (packet *)buffer;

        // Debug
        std::cout << "Id: " << receivedPacket->msgId << " Bytes: " << readBytes << " Payload: " << receivedPacket->_payload << std::endl;

        // Clear buffer
        for (int i = 0 ; i < readBytes; i++) {
            buffer[i] = '\0';
        }
    }

    // Free received argument
    free(arg);

    // Remove socket from the threads list
    Server::connection_handler_threads.erase(socket);

    // Exit
    pthread_exit(NULL);
}

void Server::setupConnection()
{
    // Create socket
    if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("Error during socket creation");

    // Prepare server socket address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(SERVER_PORT);

    // Set socket options
    int reuse = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
        throw std::runtime_error("Error setting socket options");

    // Bind socket
    if (bind(_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        throw std::runtime_error("Error during socket bind");
}