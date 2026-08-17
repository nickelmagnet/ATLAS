#pragma once
#include "db.h"

// Parses argv (excluding program name) and runs the matching command
// against the given database. Returns process exit code.
int run_cli(int argc, char** argv, Database& db);
