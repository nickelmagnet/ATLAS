#include <iostream>
#include <string>

void add(std::string task) {
	std::cout << "Adding task: " << task << "\n";
	
}

void help() {
	std::cout << "ATLAS\nA Tiny Little Assistant System\n";
	std::cout << "Usage: atlas <command>[options][arguments]\n";
	std::cout << "I haven't decided more lol " << "\n";
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
		help();
	}
	else if (argc >= 2) {
		if( std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h" ) {
			help();
		}
		else if (std::string(argv[1]) == "--version" || std::string(argv[1]) == "-v" ) {
			std::cout << "version 0.0.1\n";
		}
		else if (std::string(argv[1]) == "add") {
			if (argc == 3) {
				std::string task = argv[2];
				add(task);
			}
			else std::cout << "Give me a task dumbass.\n";

		}
	}
    return 0;
}

