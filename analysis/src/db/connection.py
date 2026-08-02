import psycopg2

def get_connection():
    conn = psycopg2.connect(
        host="localhost",
        port=5432,
        dbname="ai_sysmon",
        user="postgres",
        password="Sabaok13"
    )
    return conn


def get_recent_metrics(limit=10):
    conn = get_connection()
    cur = conn.cursor()

    cur.execute(
        "SELECT id, machine_id, collected_at, cpu_percent, ram_used_mb, ram_total_mb "
        "FROM system_metrics "
        "ORDER BY collected_at DESC "
        "LIMIT %s",
        (limit,)
    )

    rows = cur.fetchall()

    cur.close()
    conn.close()

    return rows

def insert_alert(machine_id, metric_name, metric_value, z_score, severity, message):
    conn = get_connection()
    cur = conn.cursor()

    cur.execute(
        "INSERT INTO alerts (machine_id, metric_name, metric_value, z_score, severity, message) "
        "VALUES (%s, %s, %s, %s, %s, %s)",
        (machine_id, metric_name, metric_value, z_score, severity, message)
    )

    conn.commit()  # writes are not saved until you commit
    conn.close()

def get_latest_metrics():
    conn = get_connection()
    cur = conn.cursor()

    cur.execute(
        "SELECT cpu_percent, ram_used_mb, ram_total_mb, disk_used_gb, disk_total_gb "
        "FROM system_metrics "
        "ORDER BY collected_at DESC "
        "LIMIT 1"
    )

    row = cur.fetchone()
    conn.close()
    return row

def get_cpu_history(limit=30):
    conn = get_connection()
    cur = conn.cursor()

    cur.execute(
        "SELECT cpu_percent FROM system_metrics "
        "ORDER BY collected_at DESC "
        f"LIMIT {limit}"
    )

    rows = cur.fetchall()  # list of tuples, e.g. [(2.81,), (4.69,), ...]
    conn.close()

    # rows are newest-first; reverse so the chart reads left-to-right in time order
    values = [row[0] for row in reversed(rows)]
    return values


if __name__ == "__main__":
    metrics = get_recent_metrics(10)
    for row in metrics:
        print(row)