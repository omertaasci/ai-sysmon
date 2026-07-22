#include "db.h"
#include <libpq-fe.h> // postgresql client library 
#include <stdio.h>


// holds the connection to postgresql. only this file needs to see it
static PGconn *conn = NULL;


int db_connect(void) // connect to the postgresql database
{
    // ask sql to create a connection
    // PQconnectddb() returns a pointer to a PGconn object 
    // that represents the database connection
    conn = PQconnectdb(
        "host=localhost port=5432 dbname=ai_sysmon user=postgres password=Sabaok13"
    );

    if (PQstatus(conn) != CONNECTION_OK) // check if the connection succeeded
    {
        printf("DB connection failed: %s\n", PQerrorMessage(conn));
        PQfinish(conn); // free the connection object
        conn = NULL; // avoid leaving a dangling object

        return 0; // means connection failed
    }

    printf("Connected to PostgreSQL successfully.\n");
    return 1; // means success
}

void db_disconnect(void) // close db connection
{
    if (conn != NULL) // only disconnect if a connection exists
    {
        PQfinish(conn); // close the postgresql connection
        conn = NULL; // mark the pointer as no longer valid
    }
}

// insert one row of system metrics into the database
int db_insert_metrics(double cpu_percent, unsigned long long ram_used_mb, unsigned long long ram_total_mb,
                       double disk_used_gb, double disk_total_gb)
{
    if (conn == NULL) // no db connection, cannot insert anything
    {
        return 0;
    }

    const char *sql = 
        // $1, $2, $3, $4 are placeholders, The real values will be provided separately
        // using parameters is safer than building sql strings manually
        "INSERT INTO system_metrics (machine_id, collected_at, cpu_percent, ram_used_mb, ram_total_mb, disk_used_gb, disk_total_gb) "
        "VALUES ($1, now(), $2, $3, $4, $5, $6)";

    char machine_id_str[16]; // buffer to hold machine id as text
    char cpu_str[32]; // buffer to hold spu percentage as text
    char ram_used_str[32]; // buffer to hold used ram as text
    char ram_total_str[32]; // buffer to hold total ram as text
    char disk_used_str[32]; // buffer to hold used disk as text
    char disk_total_str[32]; // buffer to gold total disk as text

    // convert int 1 into a str
    // snprintf() is safer than sprintf() because it prevents buffer overflow
    snprintf(machine_id_str, sizeof(machine_id_str), "%d", 1);

    snprintf(cpu_str, sizeof(cpu_str), "%.2f", cpu_percent); // convert cpu percentage into text with 2 decimal places
    snprintf(ram_used_str, sizeof(ram_used_str), "%llu", ram_used_mb); // convert used ram into text
    snprintf(ram_total_str, sizeof(ram_total_str), "%llu", ram_total_mb); // convert total ram into text
    snprintf(disk_used_str, sizeof(disk_used_str), "%.2f", disk_used_gb); // convert used disk into text 
    snprintf(disk_total_str, sizeof(disk_total_str), "%.2f", disk_total_gb); // convert total disk into text

    // array containing all parameter values
    // sql will replace:
    // $1 -> machine_id_str
    // $2 -> cpu_str
    // $3 -> ram_used_str
    // $4 -> ram_total_str
    const char *params[6] = {machine_id_str, cpu_str, ram_used_str, ram_total_str, disk_used_str, disk_total_str};

    PGresult *res = PQexecParams(
        conn,
        sql,
        6,        // number of parameters
        NULL,     // let PostgreSQL guess parameter types
        params,   // the actual values, as text
        NULL,     // parameter lengths (not needed for text format)
        NULL,     // parameter formats (not needed for text format)
        0         // 0 = we want text result format, 0 means return text.
    ); // execute the SQL query

    if (PQresultStatus(res) != PGRES_COMMAND_OK) // check if INSERT succeeded
    {
        printf("Insert failed: %s\n", PQerrorMessage(conn));
        PQclear(res);  // free the result obj
        return 0; // insert failed
    }
    
    // free the result object
    // every PGresult returned by postgresql should be released when finished.
    PQclear(res);
    
    return 1; // INSERT successful
}

int db_insert_processes(ProcessInfo *list, int count) {
    if (conn == NULL) {
        return 0;
    }

    for (int i = 0; i < count; i++) {
        const char *sql =
            "INSERT INTO processes (machine_id, collected_at, pid, process_name) "
            "VALUES ($1, now(), $2, $3)";

        char machine_id_str[16];
        char pid_str[16];

        snprintf(machine_id_str, sizeof(machine_id_str), "%d", 1);
        snprintf(pid_str, sizeof(pid_str), "%lu", list[i].pid);

        const char *params[3] = { machine_id_str, pid_str, list[i].name };

        PGresult *res = PQexecParams(
            conn, sql, 3, NULL, params, NULL, NULL, 0
        );

        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            printf("Process insert failed: %s\n", PQerrorMessage(conn));
            PQclear(res);
            return 0;
        }

        PQclear(res);
    }

    return 1;
}