#include "network.h"
#include <windows.h>
#include <iphlpapi.h> // ip helper api, gives network adapter statistics

void get_network_usage(unsigned long long *bytes_sent, unsigned long long *bytes_recv)
{
    // start totals at 0 before adding up all adapters
    *bytes_sent = 0;
    *bytes_recv = 0;

    MIB_IFTABLE *ifTable = NULL; // will point to the table windows fills with adapter info
    ULONG size = 0; // will hold how many bytes we need to allocate for the table

    // first call: we pass NULL as the buffer on purpose
    // this just asks windows how mant bytes you need for this table
    // windows writes the required size into size and returns an error code
    // (which we ignore here, since failing is expected on this first call)
    GetIfTable(NULL, &size, FALSE);

    // allocate exaclt 'size' bytes on the heap, now that we know how much we need
    // we use malloc because we dont know the size
    // untill program actually runs (depends on how many adapters this pc has).
    ifTable = (MIB_IFTABLE *)malloc(size);

    // always check malloc's result. if the system was out of memory, malloc returns NULL,
    // and using a NULL pointer next would crash the program
    if (ifTable == NULL)
    {
        return;
    }

    // second call: now we give it a real buffer of the correct size,
    // so windows can actually write adapter data into it
    if (GetIfTable(ifTable, &size, FALSE) == NO_ERROR)
    {
        // loop through every network interface(adapter) windows found
        for (DWORD i = 0; i < ifTable->dwNumEntries; i++)
        {
            // "Octets" is a networking term for "bytes".
            // dwOutOctets = bytes sent, dwInOctets = bytes received, for this adapter
            // we add each adapter's numbers to our running totals
            *bytes_sent += ifTable->table[i].dwOutOctets;
            *bytes_recv += ifTable->table[i].dwInOctets;
        }
    }
    // IMPORTANT: since we used malloc, we must free the memory ourself
    // forgetting this causes a memory leak -a little more ram waster every time this func
    // runs, which matters a lot since our loop runs forever
    free(ifTable);
    
}

