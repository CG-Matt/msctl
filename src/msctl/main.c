#include <stdio.h>
#include <stdlib.h>
#include "../config.h"
#include "args_parser.h"
#include "../socket_manager/sockmgr.h"

//
//  --- GLOBAL VARIABLES ---
//

static const char* g_executable_name = NULL;

// For testing all allowed input will be '-' or '+'

#define MSG_UP      "CTL BR UP"
#define MSG_DOWN    "CTL BR DN"

/**
 * Display a usage message and exit the program
 */
static void UsageExit()
{
    printf("Usage: %s [OPTIONS]\n", g_executable_name);
    printf("  OPTIONS:\n");
    printf("    --brightness        Change brightness of display\n");
    printf("    --up                Turn brightness up\n");
    printf("                          Only valid if --brightness specified before\n");
    printf("    --down              Turn brightness down\n");
    printf("                          Only valid if --brightness specified before\n");
    printf("\n");

    exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
    g_executable_name = argv[0];    // Set the global executable name variable

    if(argc < 2) UsageExit();       // If no arguments have been provided exit displaying usage.

    // Remove executable name from args and pass to parser
    // Exit if error occurs while parsing
    struct Arguments args;
    if(!ParseArguments(&args, argc - 1, argv + 1))
        UsageExit();

    // Check that a command was selected
    if(args.command == CMD_NULL)
    {
        fprintf(stderr, "ERROR: No command provided.\n");
        UsageExit();
    }

    // If the command is brightness, check that direction is set
    if(args.command == CMD_BRIGHTNESS && args.brightness_direction == B_NULL)
    {
        fprintf(stderr, "ERROR: Invalid brightness direction selected.\n");
        UsageExit();
    }

    // Create new socket for connection
    SocketUnix sock = SocketUnixNew(SOCKET_PATH);

    // Variable to store an error if it occurs
    int err;

    // Connect to socket
    err = SocketUnixConnect(sock);

    if(err)
    {
        fprintf(stderr, "ERROR: Unable to connect to socket.\n");
        return EXIT_FAILURE;
    }

    switch(args.command)
    {
        case CMD_BRIGHTNESS:
            if(args.brightness_direction == B_UP)
            {
                err = SocketUnixWriteStr(sock, MSG_UP);
                if(err)
                    fprintf(stderr, "ERROR: Unable to write message to socket.\n");
            }
            if(args.brightness_direction == B_DOWN)
            {
                err = SocketUnixWriteStr(sock, MSG_DOWN);
                if(err)
                    fprintf(stderr, "ERROR: Unable to write message to socket.\n");
            }
            break;

        case CMD_NULL:
            fprintf(stderr, "ERROR: Unreachable: CMD_NULL.\n");
    }    

    SocketUnixClose(sock);

    return EXIT_SUCCESS;
}