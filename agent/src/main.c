#include <stdio.h>
#include <windows.h>
#include "cpu.h"
#include "ram.h"

int main(void)
{
    printf("sysmon_agent starting ...\n");
    
    if (!db_connect())
    {
        printf("Could not connect to database. Exiting.\n");
        return 1;
    }
    
    while (1)
    {
        double cpu = get_cpu_usage();

        unsigned long long ram_used, ram_total;
        get_ram_usage(&ram_used, &ram_total);

        // %llu is the printf format for unsigned long long
        printf("CPU usage: %.2f%% | RAM: %llu MB / %llu MB\n", cpu, ram_used, ram_total);

        int inserted = 0;
        int attempts = 0;
        const int MAX_ATTEMPTS = 3;

        while (attempts < MAX_ATTEMPTS && !inserted)
        {
            inserted = db_insert_metrics(cpu, ram_used, ram_total);
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