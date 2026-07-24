import sys
import os

# adding (src/) folder to search path, so I can import db.connection module that is in src/db/
sys.path.append(os.path.join(os.path.dirname(__file__), ".."))

import pandas as pd
from db.connection import get_connection

def analyze_recent_metrics(limit=100):
    conn = get_connection()

    # pandas can run a SQL query directly and gives a DataFrame
    query = (
        "SELECT collected_at, cpu_percent, ram_used_mb, ram_total_mb "
        "FROM system_metrics "
        "ORDER BY collected_at DESC "
        f"LIMIT {limit}"
    )

    df = pd.read_sql(query, conn)

    conn.close()

    print(f"Analyzed {len(df)} rows\n")

    print("CPU usage stats:")
    print(f"  mean: {df['cpu_percent'].mean():.2f}%")
    print(f"  min:  {df['cpu_percent'].min():.2f}%")
    print(f"  max:  {df['cpu_percent'].max():.2f}%")
    print(f"  std:  {df['cpu_percent'].std():.2f}")

    print("\nRAM usage stats (MB):")
    print(f"  mean: {df['ram_used_mb'].mean():.2f}")
    print(f"  min:  {df['ram_used_mb'].min()}")
    print(f"  max:  {df['ram_used_mb'].max()}")
    print(f"  std:  {df['ram_used_mb'].std():.2f}")

    return df

if __name__ == "__main__":
    analyze_recent_metrics(100)