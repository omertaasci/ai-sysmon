import sys
import os

sys.path.append(os.path.join(os.path.dirname(__file__), "..", "src", "insights"))

from anomaly import compute_zscore


def test_zscore_normal_case():
    # value exactly 2 std above the mean should give z-score of 2.0
    result = compute_zscore(value=12, mean=10, std=1)
    assert result == 2.0


def test_zscore_zero_std_returns_none():
    # no variation in history -> we can't compute a meaningful z-score
    result = compute_zscore(value=5, mean=5, std=0)
    assert result is None