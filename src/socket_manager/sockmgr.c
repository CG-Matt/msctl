#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// #include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "sockmgr.h"

//
//  --- LOCAL DEFINES ---
//

#define SUCCESS             0
#define FAILURE             1

//
//  --- FUNCTIONS ---
//

SocketUnix SocketUnixNew(const char* path)
{
    SocketUnix sock;

    // Open socket for communication
    sock.fd = socket(AF_UNIX, SOCK_STREAM, 0);

    // Allocate memory for structure to store address information
    sock.impl = malloc(sizeof(struct sockaddr_un));

    // Set the contents of that struct to 0
    memset(sock.impl, 0, sizeof(struct sockaddr_un));

    // Set socket family
    ((struct sockaddr_un*)sock.impl)->sun_family = AF_UNIX;

    // Copy socket path to the address struct
    strncpy(((struct sockaddr_un*)sock.impl)->sun_path, path, sizeof(((struct sockaddr_un*)sock.impl)->sun_path) - 1);

    return sock;
}

int SocketUnixConnect(SocketUnix socket)
{
    return connect(socket.fd, (struct sockaddr*)socket.impl, sizeof(struct sockaddr_un)) == 0 ? SUCCESS : FAILURE;
}

int SocketUnixClose(SocketUnix socket)
{
    int ret = close(socket.fd) == 0 ? SUCCESS : FAILURE;

    free(socket.impl);

    return ret;
}

int SocketUnixWriteStr(SocketUnix socket, const char* data)
{
    return write(socket.fd, data, strlen(data)) >= 0 ? SUCCESS : FAILURE;
}
