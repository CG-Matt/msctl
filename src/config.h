#pragma once

/**
 * Header file containing constants used by both the root and user programs
**/

// Path to socket file used for communication between programs
#define SOCKET_PATH                 "/tmp/msctl-service.sock"

// Path to the log file used by the deamon process
#define LOG_PATH                    "/var/log/msctl.log"

// Paths used for reading / controlling the brightness of the screen
#define BRIGHTNESS_DIR              "/sys/class/backlight/intel_backlight/"
#define BRIGHTNESS_SET_PATH         BRIGHTNESS_DIR "brightness"
#define BRIGHTNESS_MAX_PATH         BRIGHTNESS_DIR "max_brightness"

// Boundry value

#define BRIGHTNESS_STEP             250
#define BRIGHTNESS_MIN              250