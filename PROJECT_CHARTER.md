# PROJECT_CHARTER.md

# 1. Project Title

AI-Powered Windows System Monitoring and Analysis Platform

---

# 2. Problem Statement

Modern computer systems generate large amounts of performance and system data such as CPU usage, memory consumption, disk activity, network traffic, and process information.

System administrators and developers need monitoring systems to detect performance problems, resource bottlenecks, and abnormal system behavior before they cause crashes or downtime.

Professional monitoring platforms such as Prometheus, Datadog, and Zabbix provide these capabilities, but they are often too complex for educational purposes.

The goal of this project is to build a simplified monitoring platform that collects low-level Windows system metrics, stores them in a PostgreSQL database, and prepares the foundation for future AI-based anomaly detection and visualization systems.

This project is also intended to improve knowledge in:

* Systems Programming
* Windows API
* Database Design
* Software Architecture
* Real-Time Monitoring Systems
* AI Integration

---

# 3. Functional Requirements (FR)

FR1: The system shall collect CPU usage information on Windows.

FR2: The system shall collect RAM usage information on Windows.

FR3: The system shall collect disk usage information on Windows.

FR4: The system shall collect network activity information on Windows.

FR5: The system shall collect process information on Windows.

FR6: The system shall store collected metrics in PostgreSQL.

FR7: The system shall allow Python applications to read stored monitoring data.

FR8: The system shall store historical monitoring data.

FR9: The system shall display real-time metrics in a desktop application.

FR10: The system shall generate alerts when abnormal behavior is detected.

FR11: The system shall support future AI-based anomaly detection modules.

---

# 4. Non-Functional Requirements (NFR)

NFR1: The monitoring agent should use minimal system resources.

NFR2: The monitoring process should not noticeably affect system performance.

NFR3: The system should remain stable even if PostgreSQL becomes temporarily unavailable.

NFR4: The codebase should be modular and maintainable.

NFR5: The application should remain responsive while collecting real-time data.

NFR6: The architecture should support future scalability and feature expansion.

NFR7: The project should follow clean coding and software engineering principles.

NFR8: The architecture should support future Linux integration with minimal changes outside the monitoring module.

---

# 5. Out of Scope (Current Version)

The following features are not included in the initial version of the project:

* Distributed multi-machine monitoring
* Cloud deployment
* User authentication and authorization
* Mobile application
* Remote agent management
* Enterprise-scale infrastructure support
* Advanced machine learning and deep learning models
* Full Linux implementation

---

# 6. MVP Definition

The Minimum Viable Product (MVP) will include:

1. A C-based monitoring agent that collects CPU and RAM usage on Windows.
2. PostgreSQL integration for storing collected metrics.
3. A Python module that reads monitoring data from PostgreSQL.
4. Console output showing the collected metrics.

The MVP goal is to validate the complete monitoring pipeline before introducing AI analysis, anomaly detection, desktop interfaces, and advanced visualization systems.

---

# 7. Success Criteria

The project will be considered successful when:

* CPU and RAM metrics are collected successfully every 5 seconds.
* The monitoring agent runs continuously for at least 30 minutes without crashing.
* Collected metrics are stored successfully in PostgreSQL without data loss.
* Python can read stored monitoring data correctly from PostgreSQL.
* The system successfully displays collected metrics through console output.
* The project structure remains modular and organized.
* The application runs successfully on Windows 11.
* The project documentation is understandable and complete on GitHub.
