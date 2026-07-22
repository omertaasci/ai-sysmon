#ifndef PROCESS_H
#define PROCESS_H

typedef struct
{
    unsigned long pid;
    char name[260];
} ProcessInfo;

int get_process_list(ProcessInfo *list, int max_count);

#endif