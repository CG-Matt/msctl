#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <sys/stat.h>
#include "filemgr.h"
#include "../config.h"

//
//  --- LOCAL DEFINITIONS ---
//

#define SUCCESS         0
#define FAILURE         1

//
//  --- LOCAL VARIABLES ---
//

static FILE* g_log_file = NULL;
static struct tm g_local_time;

//
//  --- LOCAL HELPER FUNCTIONS ---
//

/**
 * Puts the current time in the `g_local_time` struct
 */
static void FetchTime()
{
    time_t timer = time(NULL);
    g_local_time = *localtime(&timer);
}

inline static int GetHours() { return g_local_time.tm_hour; }
inline static int GetMinutes() { return g_local_time.tm_min; }
inline static int GetSeconds() { return g_local_time.tm_sec; }

//
//  --- FUNCTIONS ---
//

int StartLog(void)
{
    // Open Log File, overrides previous contents.
    g_log_file = fopen(LOG_PATH, "w");

    // Ensure log file opened successfully
    if(!g_log_file) return FAILURE;

    return SUCCESS;
}

void Log(const char* msg)
{
    // Message format
    // [HH:mm:ss] Message goes here
    FetchTime();
    fprintf(g_log_file, "[%02i:%02i:%02i] %s\n", GetHours(), GetMinutes(), GetSeconds(), msg);
    fflush(g_log_file);
}

void Logf(const char* fmt, ...)
{
    FetchTime();
    fprintf(g_log_file, "[%02i:%02i:%02i] ", GetHours(), GetMinutes(), GetSeconds());

    va_list args;
    va_start(args, fmt);
    vfprintf(g_log_file, fmt, args);
    va_end(args);

    fputc('\n', g_log_file);
    fflush(g_log_file);
}

int EndLog(void)
{
    return fclose(g_log_file) == 0 ? SUCCESS : FAILURE;
}

void LogExit(const char* msg)
{
    Log(msg);
    EndLog();
    exit(EXIT_FAILURE);
}

int FileDescriptorClose(int fd)
{
    int ret = close(fd);

    if(!ret)
        Logf("Successfully closed file: fd=%i.", fd);
    else
        Logf("Unable to close file: fd=%i.", fd);

    return ret == 0 ? SUCCESS : FAILURE;
}

FILE* FileOpenRead(const char* path)
{
    FILE* f = fopen(path, "r");

    if(f)
        Logf("Opened file '%s' for reading -> %p.", path, f);
    else
        Logf("Unable to open file '%s' for reading.", path);
    
    return f;
}

FILE* FileOpenWrite(const char* path)
{
    FILE* f = fopen(path, "w");

    if(f)
        Logf("Opened file '%s' for writing -> %p.", path, f);
    else
        Logf("Unable to open file '%s' for writing.", path);

    return f;
}

int FileSetPermissions(const char* path, unsigned int mode)
{
    int ret = chmod(path, mode);

    if(ret)
        Logf("Unable to change file permissions on '%s'.", path);

    return ret == 0 ? SUCCESS : FAILURE;
}

int FileDelete(const char* path)
{
    int ret = unlink(path);

    if(ret)
        Logf("Unable to delete file '%s': %s.", path, strerror(errno));

    return ret == 0 ? SUCCESS : FAILURE;
}

int FileClose(FILE* file)
{
    // Yes this is scuffed, but it stops gcc from complaining about use after free
    size_t p = (size_t)file;

    int ret = fclose(file);

    if(!ret)
        Logf("Successfully closed file: %p.", (void*)p);
    else
        Logf("Unable to close file: %p.", (void*)p);

    return ret == 0 ? SUCCESS : FAILURE;
}