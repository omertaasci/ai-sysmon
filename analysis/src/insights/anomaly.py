import sys
import os

# letting this file find the db.connection and config modules
sys.path.append(os.path.join(os.path.dirname(__file__), ".."))

import pandas as pd
from db.connection import get_connection, insert_alert
from config import load_config

def compute_zscore(value, mean, std):
    if std == 0:
        return None
    return (value - mean) / std

def detect_anomaly(column="cpu_percent", history_size=100, z_threshold=None):
    config = load_config()
    if z_threshold is None:
        z_threshold = config["anomaly_z_threshold"]

    conn = get_connection()

    query = (
        f"SELECT collected_at, {column} "
        "FROM system_metrics "
        "ORDER BY collected_at DESC "
        f"LIMIT {history_size}"
    )

    df = pd.read_sql(query, conn)
    conn.close()

    # df is ordered newest-first, so row 0 is the newest reading
    newest_value = df[column].iloc[0]

    # everything except the newest row becomes our history / baseline
    history = df[column].iloc[1:]

    mean = history.mean()
    std = history.std()

    # avoid dividing by zero if std happens to be 0 (e.g all values identical)
    if std == 0:
        print("Not enough variation in history to compute a z-score.")
        return

    z_score = compute_zscore(newest_value, mean, std)
    if z_score is None:
        print("Not enough variation in history to compute a z-score.")
        return

    print(f"Newest {column}: {newest_value}")
    print(f"History mean: {mean:.2f}, history std: {std:.2f}")
    print(f"Z-score: {z_score:.2f}")

    if abs(z_score) > z_threshold:
        severity = "critical" if abs(z_score) > 5 else "warning"
        message = f"{column} anomaly: value={newest_value:.2f}, z-score={z_score:.2f}"

        print(f"⚠️ ANOMALY DETECTED in {column}!")
        print(message)

        insert_alert(
            machine_id=config["machine_id"],
            metric_name=column,
            metric_value=float(newest_value),
            z_score=float(z_score),
            severity=severity,
            message=message
        )
    else:
        print(f"{column} looks normal.")

if __name__ == "__main__":
    detect_anomaly("cpu_percent")