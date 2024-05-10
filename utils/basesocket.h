#ifndef BASESOCKET_H
#define BASESOCKET_H

#include <sys/socket.h>
#include <string>
#include <chrono>

#include "datatypes.h"
#include "constants.h"

class BaseSocket {
    protected:
    struct sockaddr_in server_address;      // Server socket address
    struct sockaddr_in client_address;      // Client socket address

    /**
     * Sends a packet with given payload to the provided socket
     * @param socket Socket where the packet will be sent
     * @param payload Data that will be sent through that socket
     * @param payload_size Size of the data provided in payload
     * @returns Number of bytes sent
     */
    static int sendPacket(int socket, int msg_id, const char* payload, int payload_size) {
        int packet_size = -1;  // Size of packet to be sent
        int bytes_sent = -1;   // Number of bytes sent to the client

        // Prepare packet
        _packet* data = (_packet*)malloc(sizeof(*data) + sizeof(char) * payload_size);
        data->msg_id    = msg_id;
        data->sqn       = 1;  
        data->timestamp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        data->length = payload_size; 
        memcpy((char*)data->_payload, payload, payload_size);

        // Calculate packet size
        packet_size = sizeof(*data) + (sizeof(char) * payload_size);

        // Send packet
        if ((bytes_sent = send(socket, data, packet_size, 0)) <= 0){
            free(data);
            throw std::runtime_error("Unable to send message to server");
        }

        // Free memory used for packet
        free(data);

        // Return number of bytes sent
        return bytes_sent;
    }   
};

#endif