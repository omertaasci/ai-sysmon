-- machines : one row per monitored host
CREATE TABLE machines (
    id SERIAL PRIMARY KEY,
    hostname TEXT NOT NULL UNIQUE,
    os_type TEXT NOT NULL, -- e.g 'windows', 'linux'
    created_at TIMESTAMPTZ NOT NULL DEFAULT now()
);

-- system_metrics : one row per collection tick per machine
CREATE TABLE system_metrics (
    id BIGSERIAL PRIMARY KEY,
    machine_id INTEGER NOT NULL REFERENCES machines(id),
    collected_at TIMESTAMPTZ NOT NULL,
    cpu_percent DOUBLE PRECISION,
    ram_used_mb BIGINT,
    ram_total_mb BIGINT,
    disk_used_gb DOUBLE PRECISION,
    disk_total_gb DOUBLE PRECISION,
    net_bytes_sent BIGINT,
    net_bytes_recv BIGINT   
);

-- processes : one row per process per snapshot
CREATE TABLE processes (
    id BIGSERIAL PRIMARY KEY,
    machine_id INTEGER NOT NULL REFERENCES machines(id),
    collected_at TIMESTAMPTZ NOT NULL,
    pid INTEGER NOT NULL,
    process_name TEXT NOT NULL,
    cpu_percent DOUBLE PRECISION,
    ram_used_mb BIGINT
);

CREATE INDEX idx_system_metrics_machine_time ON system_metrics (machine_id, collected_at);