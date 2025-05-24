#pragma once

//
//  --- Struct Declerations ---
//

// Declare that a struct called 'FILE' exists without including <stdio.h>
#ifndef _STDIO_H
struct FILE;
typedef struct FILE FILE;
#endif

//
//  --- LOG FILE FUNCTIONS ---
//

/**
 * Open log file for the program.
 * Must be called before any other file manager function calls.
 * @return 0 if successful, 1 otherwise.
 */
int StartLog(void);

/**
 * Add a message to the log file.
 */
void Log(const char* msg);

/**
 * Adds a formatted message to the log file.
 */
void Logf(const char* fmt, ...);

/**
 * Ensures all text is written out to the log file and closes the file.
 * @return 0 if successful, 1 otherwise.
 */
int EndLog(void);

/**
 * Add the given message to the log file and exit the application with `EXIT_FAILURE`
 */
void LogExit(const char* msg);

//
//  --- GENERIC FILE FUNCTIONS ---
//

/**
 * Close a file using its file descriptor
 */
int FileDescriptorClose(int fd);

/**
 * Open a file for reading.
 * @return pointer to file if successful, `NULL` otherwise.
 */
FILE* FileOpenRead(const char* path);

/**
 * Open a file for writing.
 * @return pointer to file if successful, `NULL` otherwise.
 */
FILE* FileOpenWrite(const char* path);

/**
 * Set file permissions on the given file.
 * @param path Path to the file to change permissions on.
 * @param mode The permission mode to set on the file.
 * @return 0 if successful, 1 otherwise.
 */
int FileSetPermissions(const char* path, unsigned int mode);

/**
 * Deletes file at the specified path.
 * @return 0 if successful, 1 otherwise.
 */
int FileDelete(const char* path);

/**
 * Closes a file that was previously opened.
 * @return 0 if successful, 1 otherwise.
 */
int FileClose(FILE* file);