#include "db.h"
#include "cli.h"
#include <iostream>
#include <filesystem>

int main(int argc, char** argv) {
    // DB lives next to the binary for now. Later this should probably live
    // in a proper user data dir (~/.local/share/atlas/ on Linux).
    const std::string db_path = "atlas.db";
    const std::string schema_path = "../schema.sql"; // relative to build/ dir

    try {
        Database db(db_path);

        // Applying schema on every run is harmless (CREATE TABLE IF NOT EXISTS),
        // and means you never forget to initialize a fresh DB.
        if (std::filesystem::exists(schema_path)) {
            db.init_schema(schema_path);
        }
        else {
            std::cerr << "Warning: schema.sql not found at " << schema_path
                << " — assuming DB already initialized.\n";
        }

        return run_cli(argc, argv, db);
    }
    catch (const std::exception& e) {
        std::cerr << "atlas error: " << e.what() << "\n";
        return 1;
    }
}