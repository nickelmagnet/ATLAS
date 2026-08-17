#pragma once
#include <sqlite3.h>
#include <string>
#include <vector>
#include <optional>
#include "task.h"

class Database {
public:
    explicit Database(const std::string& path);
    ~Database();

    // Non-copyable: we own a raw sqlite3* handle.
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    void init_schema(const std::string& schema_sql_path);

    int add_task(const Task& t);              // returns new id
    std::vector<Task> list_tasks(const std::string& status_filter = "pending");
    bool mark_done(int id);
    std::optional<Task> next_task();           // simplest priority/deadline pick for Phase 1

private:
    sqlite3* db_ = nullptr;
    Task row_to_task(sqlite3_stmt* stmt);
};
