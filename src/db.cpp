#include "db.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>

Database::Database(const std::string& path) {
    if (sqlite3_open(path.c_str(), &db_) != SQLITE_OK) {
        std::string err = sqlite3_errmsg(db_);
        sqlite3_close(db_);
        throw std::runtime_error("Failed to open DB: " + err);
    }
}

Database::~Database() {
    if (db_) sqlite3_close(db_);
}

void Database::init_schema(const std::string& schema_sql_path) {
    std::ifstream f(schema_sql_path);
    if (!f) throw std::runtime_error("Cannot open schema file: " + schema_sql_path);
    std::ostringstream ss;
    ss << f.rdbuf();
    std::string schema = ss.str();

    char* err_msg = nullptr;
    if (sqlite3_exec(db_, schema.c_str(), nullptr, nullptr, &err_msg) != SQLITE_OK) {
        std::string err = err_msg ? err_msg : "unknown error";
        sqlite3_free(err_msg);
        throw std::runtime_error("Schema init failed: " + err);
    }
}

int Database::add_task(const Task& t) {
    const char* sql =
        "INSERT INTO tasks (title, priority, deadline, status) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("add_task prepare failed: " + std::string(sqlite3_errmsg(db_)));
    }

    sqlite3_bind_text(stmt, 1, t.title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, t.priority);
    if (t.deadline.has_value()) {
        sqlite3_bind_text(stmt, 3, t.deadline->c_str(), -1, SQLITE_TRANSIENT);
    } else {
        sqlite3_bind_null(stmt, 3);
    }
    sqlite3_bind_text(stmt, 4, t.status.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::string err = sqlite3_errmsg(db_);
        sqlite3_finalize(stmt);
        throw std::runtime_error("add_task insert failed: " + err);
    }
    sqlite3_finalize(stmt);

    return static_cast<int>(sqlite3_last_insert_rowid(db_));
}

Task Database::row_to_task(sqlite3_stmt* stmt) {
    Task t;
    t.id = sqlite3_column_int(stmt, 0);
    t.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    t.priority = sqlite3_column_int(stmt, 2);

    const unsigned char* deadline_text = sqlite3_column_text(stmt, 3);
    if (deadline_text) {
        t.deadline = std::string(reinterpret_cast<const char*>(deadline_text));
    }

    t.status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
    t.created_at = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
    return t;
}

std::vector<Task> Database::list_tasks(const std::string& status_filter) {
    std::vector<Task> results;
    std::string sql =
        "SELECT id, title, priority, deadline, status, created_at FROM tasks";
    if (status_filter != "all") {
        sql += " WHERE status = ?";
    }
    sql += " ORDER BY priority ASC, deadline ASC;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("list_tasks prepare failed: " + std::string(sqlite3_errmsg(db_)));
    }
    if (status_filter != "all") {
        sqlite3_bind_text(stmt, 1, status_filter.c_str(), -1, SQLITE_TRANSIENT);
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        results.push_back(row_to_task(stmt));
    }
    sqlite3_finalize(stmt);
    return results;
}

bool Database::mark_done(int id) {
    const char* sql = "UPDATE tasks SET status = 'done' WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("mark_done prepare failed: " + std::string(sqlite3_errmsg(db_)));
    }
    sqlite3_bind_int(stmt, 1, id);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    int changed = sqlite3_changes(db_);
    sqlite3_finalize(stmt);
    return ok && changed > 0;
}

std::optional<Task> Database::next_task() {
    // Phase 1: simplest possible pick — highest priority (lowest number),
    // earliest deadline first, among pending tasks. No dependency-awareness
    // yet — that's Phase 2's job.
    auto pending = list_tasks("pending");
    if (pending.empty()) return std::nullopt;
    return pending.front();  // already ORDER BY priority, deadline
}
