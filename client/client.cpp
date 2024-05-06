#include "client.h"

std::atomic<bool> Client::stop_issued;
int Client::server_socket;
pthread_t Client::input_handler_thread;

Client::Client(std::string server_ip, int server_port)
{
    this->server_ip = server_ip;
    this->server_port = server_port;
    this->server_socket = -1;

    // Set atomic flag as false
    stop_issued = false;
};

Client::~Client()
{
    // Close server socket if it is still open
    if (server_socket > 0)
        close(server_socket);
};

void Client::setupConnection()
{
    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("Error during socket creation");

    // Fill server socket address
    server_address.sin_family = AF_INET;
    server_address.sin_port   = htons(server_port);
    server_address.sin_addr.s_addr = inet_addr(server_ip.c_str());

    // Try to connect to remote server
    if (connect(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        throw std::runtime_error("Error connecting to server");

    // Start user input getter thread
    pthread_create(&input_handler_thread, NULL, handleUserInput, NULL);

    while(!stop_issued){

    }
};

void *Client::handleUserInput(void* arg)
{
    std::string message;
    while(std::getline(std::cin, message)) {
        try {
            // Don't send empty messages
            if (message.length() > 0)
                sendMessage(message);

        }
        catch(const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    // Signal thread to stop
    stop_issued = true;

    // Close listening socket
    shutdown(server_socket, SHUT_RDWR);

    // End with no return value
    pthread_exit(NULL);
};

int Client::sendMessage(std::string message)
{
    int bytes_sent = -1;
    const char *buf = message.c_str();
    int len = strlen(buf);

    // Send message
    if ((bytes_sent = send(server_socket, buf, len, 0)) <= 0)
        throw std::runtime_error("Unable to send message to server");

    // Return number of bytes sent
    return bytes_sent;
};