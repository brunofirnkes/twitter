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

void* Client::handleUserInput(void* arg) {
    std::string message;

    while(std::getline(std::cin, message)) {
        try {
            Command command = getCommand(message);

            // Check if command is valid before sending
            if (command != COMM_UNKNOWN) {
                const char* payload = message.c_str();
                sendPacket(_socket, (int)command, payload, strlen(payload));
            } else {
                std::cerr << "Unknown command: " << message << std::endl;
            }
        }
        catch(const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    // Signal thread to stop
    stop_issued = true;

    // Close listening socket
    shutdown(_socket, SHUT_RDWR);

    // End with no return value
    pthread_exit(NULL);
}

void Client::recvMessages() {
    int readBytes = -1;
    char buffer[BUFFER_SIZE];
    _packet* receivedPacket{nullptr};

    while(!stop_issued && (readBytes = recv(_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        // Decode message into packet format
        receivedPacket = (_packet*)buffer;

        std::cout << receivedPacket->_payload << std::endl;

        // Clear buffer
        for (int i = 0 ; i < readBytes; i++) {
            buffer[i] = '\0';
        }
    } 
}

Command Client::getCommand(std::string& input) {
    // Command for sanitizing input
    std::string comm{""};

    // Regex for matching commands
    std::regex helpRegex("^\\s*help\\s*",             std::regex_constants::icase);
    std::regex createUserRegex("^\\s*createuser\\s*", std::regex_constants::icase);

    // Match the input against the Regex
    if (std::regex_search(input, helpRegex)) {
        comm = "help";
        input = utils::trim(input.substr(input.find(comm) + comm.size()));
        return COMM_HELP;
    }
    else if (std::regex_search(input, createUserRegex)){
        comm = "createuser";
        input = utils::trim(input.substr(input.find(comm) + comm.size()));
        return COMM_CREATE_USER;
    }

    return COMM_UNKNOWN;
}