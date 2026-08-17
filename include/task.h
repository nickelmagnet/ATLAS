#pragma once
#include <string>
#include <optional>

struct Task {
    int id = -1;                       // -1 = not yet assigned by DB
    std::string title;
    int priority = 3;                  // 1 (highest) - 5 (lowest), your convention to decide
    std::optional<std::string> deadline;   // ISO 8601, e.g. "2026-08-20"
    std::string status = "pending";    // "pending" | "done"
    std::string created_at;            // filled in by DB default, read back after insert
};

// Small helper so `atlas list` has readable output.
// Defined in task.cpp.
std::string task_to_line(const Task& t);
