import sys
import os

sys.path.append(os.path.join(os.path.dirname(__file__), "..", "..", "analysis", "src"))

from PySide6.QtWidgets import QApplication, QMainWindow, QWidget, QVBoxLayout, QLabel
from PySide6.QtCharts import QChart, QChartView, QLineSeries, QValueAxis
from PySide6.QtCore import Qt, QTimer
from db.connection import get_latest_metrics, get_cpu_history
from config import load_config

config = load_config()

app = QApplication(sys.argv)

window = QMainWindow()
window.setWindowTitle("AI Sysmon Dashboard")
window.resize(800, 600)

central_widget = QWidget()
layout = QVBoxLayout()

# create labels once, with placeholder text - update_dashboard() will fill them in
cpu_label = QLabel("CPU usage: -")
ram_label = QLabel("RAM: -")
disk_label = QLabel("Disk: -")

layout.addWidget(cpu_label)
layout.addWidget(ram_label)
layout.addWidget(disk_label)

# create the chart once too; we will replace its series data on each update
series = QLineSeries()

chart = QChart()
chart.addSeries(series)
chart.setTitle("CPU Usage (last 30 readings)")
chart.legend().hide()  # we only have 1 series, legend is just noise

# create axes explicitly, once - more reliable than createDefaultAxes() for live updates
axis_x = QValueAxis()
axis_x.setTitleText("Reading #")
axis_x.setLabelFormat("%d")  # whole numbers, no decimals for reading index

axis_y = QValueAxis()
axis_y.setTitleText("CPU %")
axis_y.setRange(0, 100)  # cpu percent is always between 0 and 100 - fixed range

chart.addAxis(axis_x, Qt.AlignBottom)
chart.addAxis(axis_y, Qt.AlignLeft)
series.attachAxis(axis_x)
series.attachAxis(axis_y)

chart_view = QChartView(chart)
layout.addWidget(chart_view)

central_widget.setLayout(layout)
window.setCentralWidget(central_widget)


def update_dashboard():
    row = get_latest_metrics()

    if row is not None:
        cpu_percent, ram_used_mb, ram_total_mb, disk_used_gb, disk_total_gb = row

        cpu_label.setText(f"CPU usage: {cpu_percent:.2f}%")
        ram_label.setText(f"RAM: {ram_used_mb} / {ram_total_mb} MB")
        disk_label.setText(f"Disk: {disk_used_gb:.1f} / {disk_total_gb:.1f} GB")

    cpu_history = get_cpu_history(limit=config["chart_history_size"])

    series.clear()  # remove old points before adding new ones
    for index, value in enumerate(cpu_history):
        series.append(index, value)

    if cpu_history:
        axis_x.setRange(0, len(cpu_history) - 1)


# run once immediately, so the window isn't blank while waiting for the first tick
update_dashboard()

timer = QTimer()
timer.timeout.connect(update_dashboard)  # call update_dashboard() every tick
timer.start(config["refresh_interval_ms"])

window.show()
sys.exit(app.exec())