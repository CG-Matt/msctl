#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "../config.h"
#include "../file_manager/filemgr.h"
#include "cmd_handler.h"
#include "../common.h"

//
//  --- GLOBAL VARIABLES ---
//

static int max_brightness = 1;

//
//  --- LOCAL HELPER FUNCTIONS ---
//

/**
 * Get the maximum brightness allowed for the display.
 * Stores value in global `max_brightness`.
 * @return 0 if successful, 1 otherwise
 */
static int ReadMaxBrightness(void)
{
    // Open file containing max brightness value
    FILE* f = FileOpenRead(BRIGHTNESS_MAX_PATH);

    // Return if failed to open file
    if(!f) return 1;

    // Create buffer to store the value read from the brightness file
    char str_val_buf[64];

    // Read max brightness value from file
    size_t count = fread(str_val_buf, 1, sizeof(str_val_buf), f);

    // Close file as it is no longer needed
    FileClose(f);

    // Error if read 0 bytes
    if(count == 0)
    {
        Log("No data present in max brightness file.");
        return 1;
    }

    // Read should never be as big as the buffer, error if it is
    if(count == sizeof(str_val_buf))
    {
        Log("Value read from max brightness file is too large.");
        return 1;
    }

    // Null ternimate the read-in string
    str_val_buf[count] = '\0';

    // Convert that value into an integer
    int value = atoi(str_val_buf);

    // Do bounds checking on the value
    if(value <= 0)
    {
        Logf("Invalid value '%i' read-in from the max brightness file.", value);
        return 1;
    }
    
    // Set max_brightness value last to avoid setting too early
    Logf("Successfully read max_brightness value [%i].", value);
    max_brightness = value;
    return 0;
}

/**
 * Get the current brightness of the display
 * @return The current brightness, or -1 on failure
 */
static int get_brightness(void)
{
    // Open file containing current brightness value
    FILE* f = FileOpenRead(BRIGHTNESS_SET_PATH);

    // Return if failed to open file
    if(!f) return -1;

    // Create buffer to store the value read from the brightness file
    char str_val_buf[64];

    // Read current brightness value from file
    size_t count = fread(str_val_buf, 1, sizeof(str_val_buf), f);

    // Close file as it is no longer needed
    FileClose(f);

    // Error if read 0 bytes
    if(count == 0)
    {
        Log("No data present in brightness file.");
        return -1;
    }

    // Read should never be as big as the buffer, error if it is
    if(count == sizeof(str_val_buf))
    {
        Log("Value read from brightness file is too large.");
        return -1;
    }

    // Null ternimate the read-in string
    str_val_buf[count] = '\0';

    // Convert that value into an integer
    int value = atoi(str_val_buf);

    // Do bounds checking on the value
    if(value <= 0)
    {
        Logf("Invalid value '%i' read-in from the brightness file.", value);
        return -1;
    }
    
    return value;
}

// Rework this function, only temporary
static void change_brightness(BRIGHTNESS_OPT option)
{
    // These options will not work with this function
    if(option == B_NULL || option == B_SET) return;

    // Get the current screen brightness
    int brightness = get_brightness();

    // Return if brightness fetch failed
    if(brightness < 0) return;

    // Open brightness file for writing
    FILE* f = FileOpenWrite(BRIGHTNESS_SET_PATH);

    // Exit if unable to open file
    if(!f) return;

    // Calculate brightness level to set
    if(option == B_UP)              brightness += BRIGHTNESS_STEP;
    if(option == B_DOWN)            brightness -= BRIGHTNESS_STEP;
    if(brightness < BRIGHTNESS_MIN) brightness  = BRIGHTNESS_MIN;
    // if(brightness > brightness_max) brightness  = brightness_max; // TODO: Add max brightness limit

    // Set the brightness level
    int err = fprintf(f, "%i\n", brightness);

    // Log if we were unable to change the brightness
    if(err < 0) Log("Unable to write brightness value to file.");

    FileClose(f);
}

int main(void)
{
    // Setup local variables
    int err, root_fd, user_fd;
    struct sockaddr_un sock_addr;
    char cmd_buf[100];

    // Setup log file
    err = StartLog();

    // Exit program if unable to open the log file
    if(err)
    {
        fputs("Unable to open log file.", stderr);
        return EXIT_FAILURE;
    }

    // Setup max brightness value
    err = ReadMaxBrightness();

    // Exit program if unable to read in the max birghtness value
    if(err) LogExit("Unable to read-in max brightness value.");

    // Remove the socket file if it already exists
    err = FileDelete(SOCKET_PATH);

    // Create a socket for the root process
    root_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    
    // Check if the socket was created successfully
    if(root_fd < 0) LogExit("Unable to create socket for root process.");
    else            Log("Successfully created socket for root process.");

    // Zero the memory of the addr struct
    memset(&sock_addr, 0, sizeof(struct sockaddr_un));

    // Set the socket family to AF_UNIX
    sock_addr.sun_family = AF_UNIX;

    // Copy the socket path into the addr struct
    strncpy(sock_addr.sun_path, SOCKET_PATH, sizeof(sock_addr.sun_path) - 1);

    // Bind the socket
    err = bind(root_fd, (struct sockaddr*)&sock_addr, sizeof(struct sockaddr_un));
    
    if(err)         LogExit("Unable to bind socket to local address.");
    else            Log("Successfully bound socket to local address.");

    // Allow non-root users to connect to the socket
    FileSetPermissions(SOCKET_PATH, 0666);

    // Listen for incoming connections
    err = listen(root_fd, 5);

    // Exit program if we error trying to listen
    if(err) LogExit("Unable to listen for connections.");

    while(1)
    {
        Log("Awaiting next connection...");

        user_fd = accept(root_fd, NULL, NULL);

        if(user_fd < 0)
        {
            Log("Unable to accept connection.");
            continue;
        }
        else
        {
            Log("Accepted connection.");
        }

        // Read in data from the socket
        int bytes = read(user_fd, cmd_buf, sizeof(cmd_buf) - 1);

        if(bytes == 0)
        {
            Log("No data recieved.");
        }
        else if(bytes >= (long)(sizeof(cmd_buf) - 1))
        {
            Log("Data recieved too large, discarding.");
        }
        else
        {
            Log("Processing command");

            cmd_buf[bytes] = '\0';

            err = ParseCommand(cmd_buf);

            if(err)
            {
                Logf("Unable to process command: %s.", GetParseErrorString());
            }

            if(GetCommand() == CMD_BRIGHTNESS)
            {
                change_brightness(GetBrightnessDirection());
            }
        }

        FileDescriptorClose(user_fd);
    }

    FileDescriptorClose(root_fd);
    FileDelete(SOCKET_PATH);
    return 0;
}