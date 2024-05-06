#ifndef CLIENT_H
#define CLIENT_H

#include <regex>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <chrono>
#include <ctime>
#include <unistd.h>
#include <iostream>
#include <atomic>
#include <pthread.h>
#include "constants.h"

class Client {
    private:
    static std::atomic<bool> stop_issued;   // Atomic flag for stopping all threads
    std::string server_ip;                  // IP of the remote server
    int server_port;                        // Port the remote server listens to
    static int  server_socket;              // Socket for remote server communication
    struct sockaddr_in server_address;      // Server socket address
    static pthread_t input_handler_thread;  // Thread to listen for new incoming server messages

    /**
     * Handles getting user input for sending to server
     */
    static void *handleUserInput(void* arg);

    /**
     * Send message to server
     * @param message Message to be sent to the chatroom
     * @return Number of bytes sent to remote server
     */
    static int sendMessage(std::string message);

    public:
    /**
     * Class constructor
     * @param server_ip IP Address for the remote server, formatted in IPv4 (x.x.x.x)
     * @param server_port Port the remote server is listening at
     */
    Client(std::string server_ip, int server_port);

    /**
     * Class destructor
     * Closes communication socket with the remote server if one has been opened
     */
    ~Client();

    /**
     * Setup connection to remote server
     */
    void setupConnection();
};

#endif