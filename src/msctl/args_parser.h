#pragma once

#include "../common.h"

// These are the valid modes that the program can operate in

#define CMD_STR_BRIGHTNESS      "brightness"
#define CMD_STR_UP              "up"
#define CMD_STR_DOWN            "down"

struct Arguments
{
    COMMAND command;
    BRIGHTNESS_OPT brightness_direction;
};

int ParseArguments(struct Arguments* args_dest, int arg_count, char** args);