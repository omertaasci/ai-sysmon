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
int db_insert_metrics(double cpu_percent, unsigned long long ram_used_mb, unsigned long long ram_total_mb)
{
    if (conn == NULL) // no db connection, cannot insert anything
    {
        return 0;
    }

    const char *sql = 
        // $1, $2, $3, $4 are placeholders, The real values will be provided separately
        // using parameters is safer than building sql strings manually
        "INSERT INTO system_metrics (machine_id, collected_at, cpu_percent, ram_used_mb, ram_total_mb) "
        "VALUES ($1, now(), $2, $3, $4)";

    char machine_id_str[16]; // buffer to hold machine id as text
    char cpu_str[32]; // buffer to hold spu percentage as text
    char ram_used_str[32]; // buffer to hold used ram as text
    char ram_total_str[32]; // buffer to hold total ram as text

    // convert int 1 into a str
    // snprintf() is safer than sprintf() because it prevents buffer overflow
    snprintf(machine_id_str, sizeof(machine_id_str), "%d", 1);

    // convert cpu percentage into text with 2 decimal places
    snprintf(cpu_str, sizeof(cpu_str), "%.2f", cpu_percent);

    // convert used ram into text
    snprintf(ram_used_str, sizeof(ram_used_str), "%llu", ram_used_mb);

    // convert total ram into text
    snprintf(ram_total_str, sizeof(ram_total_str), "%llu", ram_total_mb);

    // array containing all parameter values
    // sql will replace:
    // $1 -> machine_id_str
    // $2 -> cpu_str
    // $3 -> ram_used_str
    // $4 -> ram_total_str
    const char *params[4] = {machine_id_str, cpu_str, ram_used_str, ram_total_str};

    PGresult *res = PQexecParams(
        conn,
        sql,
        4,        // number of parameters
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