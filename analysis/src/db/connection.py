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


if __name__ == "__main__":
    metrics = get_recent_metrics(10)
    for row in metrics:
        print(row)