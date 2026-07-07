#include <stdio.h>
#include <windows.h>
#include "cpu.h"
#include "ram.h"

int main(void)
{
    printf("sysmon_agent starting ...\n");
    
    while (1)
    {
        double cpu = get_cpu_usage();

        unsigned long long ram_used, ram_total;
        get_ram_usage(&ram_used, &ram_total);

        // %llu is the printf format for unsigned long long
        printf("CPU usage: %.2f%% | RAM: %llu MB / %llu MB\n", cpu, ram_used, ram_total);
    }
    
    return 0;
}