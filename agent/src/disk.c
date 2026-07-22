#include "disk.h"
#include <windows.h>

void get_disk_usage(double *used_gb, double *total_gb) // get disk usage
{
    ULARGE_INTEGER freeBytesAvailable;  // free space available to current user
    ULARGE_INTEGER totalBytes; // total size of disk
    ULARGE_INTEGER totalFreeBytes; // total free space on disk

    GetDiskFreeSpaceExA(
        "C:\\", // check C:
        &freeBytesAvailable, // windows writes available free space here
        &totalBytes, // windows writes total disk size here
        &totalFreeBytes // windows writes total free space here
    );

    double total_bytes_d = (double)totalBytes.QuadPart; // convert  total disk size from a 64bit int into a double
    double free_bytes_d = (double)totalFreeBytes.QuadPart; // convert free disk space into a double
    double used_bytes_d = total_bytes_d - free_bytes_d; // used disk space

    const double GB = 1024.0 * 1024.0 * 1024.0;

    *total_gb = total_bytes_d / GB; // total bytes to gb
    *used_gb = used_bytes_d / GB; // used bytes to gb
}