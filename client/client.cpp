#include "client.h"

std::atomic<bool> Client::stop_issued;
int Client::_socket;
pthread_t Client::input_handler_thread;

Client::Client(std::string server_ip, int server_port)
{
    this->server_ip = server_ip;
    this->server_port = server_port;
    _socket = -1;

    // Set atomic flag as false
    stop_issued = false;
};

Client::~Client()
{
    // Close server socket if it is still open
    if (_socket > 0)
        close(_socket);
};

void Client::setupConnection()
{
    // Create socket
    if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw std::runtime_error("Error during socket creation");

    // Fill server socket address
    server_address.sin_family = AF_INET;
    server_address.sin_port   = htons(server_port);
    server_address.sin_addr.s_addr = inet_addr(server_ip.c_str());

    // Try to connect to remote server
    if (connect(_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        throw std::runtime_error("Error connecting to server");

    // Start user input getter thread
    pthread_create(&input_handler_thread, NULL, handleUserInput, NULL);
};

void *Client::handleUserInput(void* arg)
{
    std::string message;

    // Read messages from client's terminal
    while(std::getline(std::cin, message)) {
        try {
            const char* payload = message.c_str();
            
            // Send packet to remote server
            sendPacket(_socket, CREATE_USER, payload, strlen(payload));
        }
        catch(const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    // Signal thread to stop
    stop_issued = true;

    // Close listening socket
    shutdown(_socket, SHUT_RDWR);

    // End with no return value
    pthread_exit(NULL);
};

void Client::recvMessages() {
    int readBytes = -1;
    int payloadBytes = 0;
    char buffer[BUFFER_SIZE];
    packet* receivedPacket{nullptr};

    while(!stop_issued && (readBytes = recv(_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        // Decode message into packet format
        receivedPacket = (packet*)buffer;

        std::cout << "Id: " << receivedPacket->msgId << " Bytes: " << readBytes << " Payload: " << receivedPacket->_payload << std::endl;

        // Clear buffer
        for (int i = 0 ; i < readBytes; i++) {
            buffer[i] = '\0';
        }
    } 
}