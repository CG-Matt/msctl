#pragma once

#include "../common.h"

/**
 * Parse the passed in command string.
 * @param command The command string to parse.
 * @return 0 if successful, 1 otherwise.
 */
int ParseCommand(const char* command);

/**
 * Returns a string describing the problem encountered during parsing.
 */
const char* GetParseErrorString(void);

/**
 * Get the command to execute
 */
COMMAND GetCommand(void);

/**
 * Get the brightness control direction
 */
BRIGHTNESS_OPT GetBrightnessDirection(void);