#include <string.h>
#include "cmd_handler.h"

//
//  --- LOCAL DEFINES ---
//

#define SUCCESS                 0
#define FAILURE                 1

#define PAR_ERR_INVALID_PREAMBLE    "Invalid command preamble"
#define PAR_ERR_INVALID_COMMAND     "Invalid command"
#define PAR_ERR_INVALID_CMD_ARG     "Invalid command argument"

//
//  --- LOCAL VARIABLES ---
//

static const char* g_parse_error_str = NULL;
static COMMAND g_cmd = CMD_NULL;
static BRIGHTNESS_OPT g_br_direction = B_NULL;

//
//  --- FUNCTIONS ---
//

int ParseCommand(const char* command)
{
    // Commands are of the form preamble .. command .. args
    // Where:
    //      preamble is 'CTL '
    //      command is of the form 'XX', where X is any uppercase character
    //      args is optional

    // Character to store command
    g_cmd = CMD_NULL;

    // First check if we have the correct preable [CTL ]
    if(strncmp(command, "CTL ", 4))
    {
        g_parse_error_str = PAR_ERR_INVALID_PREAMBLE;
        return FAILURE;
    }

    command += 4;

    // Command selection
    if(strncmp(command, "BR", 2) == 0)
    {
        g_cmd = CMD_BRIGHTNESS;
    }

    // Check if a command has been selected
    if(g_cmd == CMD_NULL)
    {
        g_parse_error_str = PAR_ERR_INVALID_COMMAND;
        return FAILURE;
    }

    command += 2;

    if(g_cmd == CMD_BRIGHTNESS)
    {
        // Check for space and an extra argument of direction
        if(*command != ' ')
        {
            g_parse_error_str = PAR_ERR_INVALID_CMD_ARG;
            return FAILURE;
        }
        
        command++;

        if(strncmp(command, "UP", 2) == 0)
        {
            g_br_direction = B_UP;
            return SUCCESS;
        }

        if(strncmp(command, "DN", 2) == 0)
        {
            g_br_direction = B_DOWN;
            return SUCCESS;
        }

        g_br_direction = B_NULL;
        g_parse_error_str = PAR_ERR_INVALID_CMD_ARG;
        return FAILURE;
    }

    return SUCCESS;
}

const char* GetParseErrorString(void){ return g_parse_error_str; }

COMMAND GetCommand(void)
{
    return g_cmd;
}

BRIGHTNESS_OPT GetBrightnessDirection(void)
{
    return g_br_direction;
}
