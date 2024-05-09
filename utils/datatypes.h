#ifndef DATATYPES_H
#define DATATYPES_H

#include <iostream>

// Packet that is sent between server and client
typedef struct _packet {
    uint16_t msgId;         // Message ID
    uint16_t sqn;           // Sequence number
    uint16_t length;        // Message lenght
    uint64_t timestamp;     // Message timestamp
    const char _payload[];  // Message payload
} packet;

typedef struct _message {
    int msgId;  // Message ID
    void* argv; // Data
} message;

#endif