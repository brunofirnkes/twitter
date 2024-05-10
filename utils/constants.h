#ifndef CONSTANTS_H
#define CONSTANTS_H

// Server related constants
#define SERVER_PORT  6000   // Port the remote server listens at
#define BUFFER_SIZE  2048   // Max buffer size

// Packet data from server to client
#define PKT_DATA  0

enum Command {
    COMM_UNKNOWN,
    COMM_HELP,
    COMM_CREATE_USER,
    LAST,
};

#endif