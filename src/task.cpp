#include "task.h"
#include <sstream>

std::string task_to_line(const Task& t) {
    std::ostringstream out;
    out << "#" << t.id << "  " << t.title
        << "\n    priority: " << t.priority
        << "\n    status:   " << t.status;
    if (t.deadline.has_value()) {
        out << "\n    deadline: " << *t.deadline;
    }
    return out.str();
}
