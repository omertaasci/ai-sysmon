#ifndef DB_H
#define DB_H

int db_connect(void);
void db_disconnect(void);
int db_insert_metrics(double cpu_percent, unsigned long long ram_used_mb, unsigned long long ram_total_mb,
                       double disk_used_gb, double disk_total_gb);
#endif