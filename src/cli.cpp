#include "cli.h"
#include <iostream>
#include <vector>
#include <string>

namespace {

void print_usage() {
    std::cout <<
        "Usage:\n"
        "  atlas add \"<title>\" [--priority N] [--deadline YYYY-MM-DD]\n"
        "  atlas list [pending|done|all]   (default: pending)\n"
        "  atlas done <id>\n"
        "  atlas next\n";
}

// Very small manual flag parser. Good enough for Phase 1 — replace with a
// real argument-parsing library later if flags multiply.
std::optional<std::string> get_flag_value(const std::vector<std::string>& args,
                                           const std::string& flag) {
    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == flag && i + 1 < args.size()) {
            return args[i + 1];
        }
    }
    return std::nullopt;
}

int cmd_add(const std::vector<std::string>& args, Database& db) {
    if (args.empty()) {
        std::cerr << "atlas add: missing title\n";
        return 1;
    }
    Task t;
    t.title = args[0];

    if (auto p = get_flag_value(args, "--priority")) {
        t.priority = std::stoi(*p);
    }
    if (auto d = get_flag_value(args, "--deadline")) {
        t.deadline = *d;
    }

    int id = db.add_task(t);
    std::cout << "Added task #" << id << "\n";
    return 0;
}

int cmd_list(const std::vector<std::string>& args, Database& db) {
    std::string filter = args.empty() ? "pending" : args[0];
    auto tasks = db.list_tasks(filter);

    if (tasks.empty()) {
        std::cout << "No " << filter << " tasks.\n";
        return 0;
    }
    for (const auto& t : tasks) {
        std::cout << task_to_line(t) << "\n\n";
    }
    return 0;
}

int cmd_done(const std::vector<std::string>& args, Database& db) {
    if (args.empty()) {
        std::cerr << "atlas done: missing task id\n";
        return 1;
    }
    int id = std::stoi(args[0]);
    if (db.mark_done(id)) {
        std::cout << "Marked #" << id << " done.\n";
        return 0;
    }
    std::cerr << "No pending task with id #" << id << "\n";
    return 1;
}

int cmd_next(Database& db) {
    auto t = db.next_task();
    if (!t.has_value()) {
        std::cout << "Nothing pending. You're clear.\n";
        return 0;
    }
    std::cout << "-> " << task_to_line(*t) << "\n";
    return 0;
}

} // namespace

int run_cli(int argc, char** argv, Database& db) {
    if (argc < 2) {
        print_usage();
        return 1;
    }

    std::string command = argv[1];
    std::vector<std::string> args(argv + 2, argv + argc);

    if (command == "add")  return cmd_add(args, db);
    if (command == "list") return cmd_list(args, db);
    if (command == "done") return cmd_done(args, db);
    if (command == "next") return cmd_next(db);

    std::cerr << "Unknown command: " << command << "\n";
    print_usage();
    return 1;
}
