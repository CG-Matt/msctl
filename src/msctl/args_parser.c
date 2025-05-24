#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "args_parser.h"

#define eprintf(args...) fprintf(stderr, args)

int ParseArguments(struct Arguments* args_dest, int argc, char** args)
{
    // Setup output struct
    args_dest->command = CMD_NULL;
    args_dest->brightness_direction = B_NULL;

    for(int i = 0; i < argc; i++)
    {
        const char* cur_arg = args[i];

        // No need to check for a minimum length of 1 as length of 0 is not possible
        if(*cur_arg == '-')
        {
            if(strlen(cur_arg) < 2)
            {
                eprintf("ERROR: No argument provided after argument marker.\n");
                return 0;
            }

            if(cur_arg[1] != '-')
            {
                // We are dealing with a single character argument.
                eprintf("TODO: Single character arguments.\n");
                exit(EXIT_FAILURE);
            }

            // We are dealing with word arguments

            // Remove the '--' at the beginning
            cur_arg += 2;

            if(strcmp(cur_arg, "brightness") == 0)
            {
                if(args_dest->command != CMD_NULL){ eprintf("ERROR: Command set more than once.\n"); return 0; }
                args_dest->command = CMD_BRIGHTNESS;
                continue;
            }
        
            if(strcmp(cur_arg, "up") == 0)
            {
                if(args_dest->command != CMD_BRIGHTNESS){ eprintf("ERROR: Unknown argument '--up'\n"); return 0; }
                if(args_dest->brightness_direction != B_NULL){ eprintf("ERROR: Brightness direction set more than once.\n"); return 0; }
                args_dest->brightness_direction = B_UP;
                continue;
            }

            if(strcmp(cur_arg, "down") == 0)
            {
                if(args_dest->command != CMD_BRIGHTNESS){ eprintf("ERROR: Unknown argument '--down'\n"); return 0; }
                if(args_dest->brightness_direction != B_NULL){ eprintf("ERROR: Brightness direction set more than once.\n"); return 0; }
                args_dest->brightness_direction = B_UP;
                continue;
            }

            eprintf("ERROR: Unknown argument '--%s'\n", cur_arg);
            return 0;
        }
        else
        {
            eprintf("ERROR: Unknown argument '%s'\n", cur_arg);
            return 0;
        }
    }

    return 1;
}