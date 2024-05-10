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

#include "../utils/constants.h"
#include "../utils/basesocket.h"
#include "../utils/datatypes.h"
#include "../utils/genericutils.h"
#include "manager.h"

class Server : BaseSocket {
    private:
    Manager* manager;                       // Manager for handling clients messages
    
    static int _socket;                     // Socket the server listens at for new incoming connections
    
    pthread_t command_handler_thread;       // Thread for handling server

    static std::atomic<bool> stop_issued;   // Atomic thread for stopping all threads
    static std::map<int, pthread_t> connection_handler_threads;

    /*
     * Sets up the server socket to begin for listening
     */
    void setupConnection();

    /*
     * Handle any incoming connections, spawned by listenConnections
     * One handleConnection thread per connected client
     */
    static void* handleConnection(void* arg);

    /*
     * Handle server administrator commands
     * This should run in a separate thread to the main server thread
     */
    static void* handleCommands(void* arg);

    public:
    /*
     * Class constructor, Initializes server socket
     */
    Server();

    /*
     * Class destructor, closes any open sockets
     */
    ~Server();

    /*
     * Listens for incoming connections from clients
     * Upon receiving a connection, spawns a new thread to handle communication
     */
    void listenConnections();
};

#endif