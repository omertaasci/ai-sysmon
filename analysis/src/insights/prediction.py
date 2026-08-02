import sys
import os

# find db.connect
sys.path.append(os.path.join(os.path.dirname(__file__), ".."))

import numpy as np
import pandas as pd
from db.connection import get_connection

def predict_trend(column="cpu_percent", history_size=50, minutes_ahead=5):
    conn = get_connection()

    query = (
        f"SELECT collected_at, {column} "
        "FROM system_metrics "
        "ORDER BY collected_at DESC "
        f"LIMIT {history_size}"
    )

    df = pd.read_sql(query, conn)
    conn.close()

    df = df.iloc[::-1].reset_index(drop=True)

    if len(df) < 2:
        print("Not enough data to predict a trend")
        return

    # convert timestamps into "seconds since the first reading"
    # polyfit needs plain numbers on the x-axis, not datetime objects
    t0 = df["collected_at"].iloc[0]
    x = (df["collected_at"] - t0).dt.total_seconds().to_numpy()
    y = df[column].to_numpy()

    # fit a straight line: y = slope * x + intercept
    # degree=1 means "a straight line" (degree=2 would be a curve)
    slope, intercept = np.polyfit(x, y, deg=1)

    print(f"Slope: {slope:.4f} {column}-units per second")

    if abs(slope) < 0.0001:
        print(f"{column} is stable, no clear trend.")
        return

    # predict the value N minutes from the last reading
    future_seconds = x[-1] + (minutes_ahead * 60)
    predicted_value = slope * future_seconds + intercept

    print(f"Predicted {column} in {minutes_ahead} min: {predicted_value:.2f}")


if __name__ == "__main__":
    predict_trend("cpu_percent")