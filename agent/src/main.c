#include <stdio.h>
#include <windows.h>
#include "cpu.h"
#include "ram.h"
#include "disk.h"
#include "db.h"
#include "process.h"
#include "network.h"

#define MAX_PROCESSES 512 // max number of processes for one cycle

int main(void)
{
    printf("sysmon_agent starting ...\n");
    
    if (!db_connect()) // try to connect to db
    {
        printf("Could not connect to database. Exiting.\n");
        return 1;
    }
    
    while (1)
    {
        double cpu = get_cpu_usage(); // getting cpu usage

        // getting ram usage
        unsigned long long ram_used, ram_total;
        get_ram_usage(&ram_used, &ram_total);

        // getting disk usage
        double disk_used, disk_total;
        get_disk_usage(&disk_used, &disk_total);

        // %llu is the printf format for unsigned long long
        printf("CPU usage: %.2f%% | RAM: %llu MB / %llu MB | Disk: %.2f GB / %.2f GB\n",
        cpu, ram_used, ram_total, disk_used, disk_total);

        ProcessInfo processes[MAX_PROCESSES]; // processes array 

        // fills processes and returns how many were found
        int process_count = get_process_list(processes, MAX_PROCESSES);
        printf("Found %d processes. First few:\n", process_count);

        // save to db
        if (!db_insert_processes(processes, process_count)) 
        {
            printf("Failed to save process list.\n");
        }

        unsigned long long net_sent, net_recv; // will hold sent/received byte totals
        get_network_usage(&net_sent, &net_recv); // ask windows for current network totals
        printf("Network: sent %llu bytes, recv %llu bytes\n", net_sent, net_recv);



        // --- save to db with retry then drop logic ---

        int inserted = 0;
        int attempts = 0;
        const int MAX_ATTEMPTS = 3;

        // keep trying
        while (attempts < MAX_ATTEMPTS && !inserted)
        {
            inserted = db_insert_metrics(cpu, ram_used, ram_total, disk_used, disk_total, net_sent, net_recv);
            if (!inserted)
            {
                attempts++;
                printf("Insert failed, retrying (%d/%d)...\n", attempts, MAX_ATTEMPTS);
                Sleep(5000);
            }
            
        }
        
        if (!inserted)
        {
            printf("Dropping this sample after %d failed attempts.", MAX_ATTEMPTS);
        }
    }
    
    db_disconnect();
    return 0;
}