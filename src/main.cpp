#include <iostream>
#include <string>

void help() {
	std::cout << "ATLAS\nA Tiny Little Operating System\n";
	std::cout << "Usage: atlas <command>[options][arguments]\n";
	std::cout << "I haven't decided more lol " << "\n";
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
		help();
	}
	else if (argc == 2) {
		if( std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h" ) {
			help();
		}
	}
	else {
		std::cout << "Work in progress.\n";
	}
    return 0;
}

