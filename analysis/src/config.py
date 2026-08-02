import json
import os

# config.json lives at the project root, two levels up from this file
_CONFIG_PATH = os.path.join(os.path.dirname(__file__), "..", "..", "config.json")


def load_config():
    with open(_CONFIG_PATH, "r") as f:
        return json.load(f)