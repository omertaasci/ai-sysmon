#include "disk.h"
#include <windows.h>

void get_disk_usage(double *used_gb, double *total_gb)
{
    ULARGE_INTEGER freeBytesAvailable;
    ULARGE_INTEGER totalBytes;
    ULARGE_INTEGER totalFreeBytes;

    GetDiskFreeSpaceExA(
        "C:\\",
        &freeBytesAvailable,
        &totalBytes,
        &totalFreeBytes
    );

    double total_bytes_d = (double)totalBytes.QuadPart;
    double free_bytes_d = (double)totalFreeBytes.QuadPart;
    double used_bytes_d = total_bytes_d - free_bytes_d;

    const double GB = 1024.0 * 1024.0 * 1024.0;

    *total_gb = total_bytes_d / GB;
    *used_gb = used_bytes_d / GB;
}