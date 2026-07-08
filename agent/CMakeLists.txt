cmake_minimum_required(VERSION 3.20)

project(sysmon_agent C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(PG_ROOT "C:/Program Files/PostgreSQL/18")

add_executable(sysmon_agent 
src/main.c
src/cpu.c
src/ram.c
src/db.c
)

target_include_directories(sysmon_agent PRIVATE include "${PG_ROOT}/include")
target_link_directories(sysmon_agent PRIVATE "${PG_ROOT}/lib")
target_link_libraries(sysmon_agent PRIVATE libpq)

add_custom_command(TARGET sysmon_agent POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PG_ROOT}/bin/libpq.dll"
        "${PG_ROOT}/bin/libintl-9.dll"
        "${PG_ROOT}/bin/libssl-3-x64.dll"
        "${PG_ROOT}/bin/libcrypto-3-x64.dll"
        $<TARGET_FILE_DIR:sysmon_agent>
)