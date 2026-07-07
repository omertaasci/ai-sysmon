#include "cpu.h"
#include <windows.h> // Needed for Windows API functions and structs like FILETIME

// FILETIME is a windows struct
// like a small box holding a big number split into two parts
// because we need before and after boxes so we can make two sets (1 and 2)

// Helper function : converts FILETIME into one normal 64 bit number
// splits a number into LowPart/HighPart, two 32bit half
static ULONGLONG filetime_to_uint64(FILETIME ft)
{
    ULARGE_INTEGER value; // Special Windows type that can hold a 64-bit number.
    value.LowPart = ft.dwLowDateTime; // Put the lower 32 bits into LowPart.
    value.HighPart = ft.dwHighDateTime; // Put the upper 32 bits into HighPart.
    return value.QuadPart; // Return the fully combined 64 bit number.
}

// Function that calculates CPU usage percentage.
double get_cpu_usage(void)
{
    FILETIME idleTime1, kernelTime1, userTime1; // First reading ("before")

    // idleTime1   = time CPU spent doing nothing
    // kernelTime1 = operating system kernel time
    // userTime1   = normal program execution time

    FILETIME idleTime2, kernelTime2, userTime2; // Second reading ("after").

    // GetSystemTimes :
    // Ask Windows for current CPU timing information.
    // & means: give the function the address of my variable so it can write values into it
    GetSystemTimes(&idleTime1, &kernelTime1, &userTime1);

    Sleep(1000); // wait 1 second between two readings. We need time to pass so the second reading changes

    GetSystemTimes(&idleTime2, &kernelTime2, &userTime2); // Get CPU timing information again after 1 second.

    ULONGLONG idle1 = filetime_to_uint64(idleTime1); // convert first idle FILETIME into normal 64 bit number
    ULONGLONG kernel1 = filetime_to_uint64(kernelTime1); // converts first kernel FILETIME
    ULONGLONG user1 = filetime_to_uint64(userTime1); // converts first user FILETIME

    ULONGLONG idle2 = filetime_to_uint64(idleTime2); // convert second idle FILETIME
    ULONGLONG kernel2 = filetime_to_uint64(kernelTime2); // converts second kernel FILETIME
    ULONGLONG user2 = filetime_to_uint64(userTime2); // converts second user FILETIME

    ULONGLONG idleDiff = idle2 - idle1; // How much idle time passed during 1 second

    // Total CPU activity during that time
    // kernel time + user time = total CPU time
    ULONGLONG totalDiff = (kernel2 - kernel1) + (user2 - user1);

    // safety check for crash cases
    // Prevent division by zero.
    // Division by zero in C is undefined behavior.
    if (totalDiff == 0)
    {
        return 0.0;
    }
    
    // totalDiff - idleDiff = actual busy CPU time
    // busy / total * 100 = CPU usage percentage
    double usage = (double)(totalDiff - idleDiff) / (double)totalDiff * 100.0;

    // Clamp: CPU usage must always be between 0 and 100
    if (usage < 0.0)
    {
        usage = 0.0;
    }

    if (usage > 100.0)
    {
        usage = 100.0;
    }
    
    

    return usage;
}