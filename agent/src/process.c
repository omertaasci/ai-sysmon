#include "process.h"
#include <windows.h>
#include <tlhelp32.h> // for ToolHelp32 snapshot api (process listening)


// takes a snapshot of all running processes right now
// then walks through it and copies pid + name into our own array
int get_process_list(ProcessInfo *list, int max_count)
{
    // ask windows for a snapshot containing all current processes
    // TH32CD_SNAPPROCESS means "I want process info" (not threads/modules)
    // the second arg (0) is ignored for process snapshots
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    // always check if the snapshot actually worked before using it
    if (snapshot == INVALID_HANDLE_VALUE)
    {
        return 0; // 0 processes returned, since we failed
    }

    PROCESSENTRY32 entry; // one slot that windows will fill in, one process at a time

    // windows requires us to tell it how big our struct is before using it
    // this is a safety check many windows apis use 
    entry.dwSize = sizeof(PROCESSENTRY32);

    int count = 0; // how many processes succesfully copied so far

    // get first process in the snapshot. fills entry if succesfull
    if (Process32First(snapshot, &entry))
    {
        // do-while: we already have the first entry loaded,
        // so we process it first, then ask for the next one each loop.
        do
        {
            // safety check: never write past the end of the array the caller gave us
            // without this, we could corrupt memory (buffer overflow)
            if (count >= max_count)
            {
                break;
            }
            
            // copy the process id into our own struct array
            list[count].pid = entry.th32ProcessID;

            // copy the process name
            // strncpy copies at most (sizeof(name)-1) characters, so we never overflow
            strncpy(list[count].name, entry.szExeFile, sizeof(list[count].name) - 1);

             // strncpy does not guarantee a '\0' at the end if the source was too long
            // so we manually force the last character to be '\0' 
            list[count].name[sizeof(list[count].name) - 1] = '\0';

            count++; // move to the next slot in our array

        // ask windowns for next process loop continues while there are more
        } while (Process32Next(snapshot, &entry));
    }
    
    CloseHandle(snapshot); // we must release the snapshot resource when done

    return count; // return how many processes actually found
    
}