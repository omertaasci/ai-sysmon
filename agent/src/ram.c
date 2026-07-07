#include "ram.h"
#include <windows.h> // Needed for Windows API functions and structs like MEMORYSTATUSEX.

// function that gets ram usage info
void get_ram_usage(unsigned long long *used_mb, unsigned long long *total_mb)
{
    // MEMORYSTATUSEX is a windows struct
    // like a big box that will contain memory information filled by windows
    MEMORYSTATUSEX status; 

    // windows requires this line.
    // we must tell Windows how large our struct is.

    // sizeof(status) = size of the struct in bytes
    // this is a safety check. without this line, GlobalMemoryStatusEx() will fail.
    status.dwLength = sizeof(status);

    // ask windows to fill our struct with real ram data
    // &status means: here is the address of my struct, write the information into it
    GlobalMemoryStatusEx(&status);

    // total physical RAM in bytes
    // e.g: 16 gb ram = large byte number here.
    unsigned long long total_bytes = status.ullTotalPhys;

    unsigned long long free_bytes = status.ullAvailPhys; // currently available/free ram in bytes

    unsigned long long used_bytes = total_bytes - free_bytes; // used ram = total ram - free ram

    // converts total ram from bytes into mb
    // 1 mb = 1024 * 1024 bytes
    // *total_mb means: write into the caller's variable
    *total_mb = total_bytes / (1024 * 1024);

    // convert used ram into mb.
    // again: used_mb writes the result into the caller's variable.
    *used_mb = used_bytes / (1024 * 1024);
}