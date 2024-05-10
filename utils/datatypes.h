#ifndef DATATYPES_H
#define DATATYPES_H

#include <iostream>

// Packet that is sent between server and client
typedef struct _packet {
    uint8_t msg_id;          // Message ID
    uint16_t sqn;           // Sequence number
    uint16_t length;        // Message lenght
    uint64_t timestamp;     // Message timestamp
    const char _payload[];  // Message payload
} _packet;

typedef struct _message {
    int msg_id;  // Message ID
    void* argv; // Data
} _message;

#endif