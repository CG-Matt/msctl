#pragma once

typedef struct {
    int fd;
    void* impl;
} SocketUnix;

/**
 * Creates a new unix socket for communication.
 */
SocketUnix SocketUnixNew(const char* path);

/**
 * Connects to the specified socket.
 * @return 0 if successful, 1 otherwise.
 */
int SocketUnixConnect(SocketUnix socket);

/**
 * Closes a previously established connection and frees all resources.
 * @return 0 if successful, 1 otherwise.
 */
int SocketUnixClose(SocketUnix socket);

/**
 * Writes data to the provided socket.
 * The socket must be connected.
 */
int SocketUnixWriteStr(SocketUnix socket, const char* data);