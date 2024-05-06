#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <chrono>
#include <iostream>
#include <pthread.h>
#include <map>
#include <atomic>

#include "constants.h"

class Server {
    private:    
    int server_socket;                      // Socket the server listens at for new incoming connections

    struct sockaddr_in server_address;      // Server socket address
    struct sockaddr_in client_address;      // Client socket address

    pthread_t command_handler_thread;       // Thread for handling server

    static std::atomic<bool> stop_issued;   // Atomic thread for stopping all threads
    static std::map<int, pthread_t> connection_handler_threads;

    /**
     * Sets up the server socket to begin for listening
     */
    void setupConnection();

    /**
     * Handle any incoming connections, spawned by listenConnections
     * One handleConnection thread per connected client
     */
    static void *handleConnection(void* arg);

    /**
     * Handle server administrator commands
     * This should run in a separate thread to the main server thread
     */
    static void *handleCommands(void* arg);

    public:
    /**
     * Class constructor, Initializes server socket
     */
    Server();

    /**
     * Class destructor, closes any open sockets
     */
    ~Server();

    /**
     * Listens for incoming connections from clients
     * Upon receiving a connection, spawns a new thread to handle communication
     */
    void listenConnections();
};

#endif