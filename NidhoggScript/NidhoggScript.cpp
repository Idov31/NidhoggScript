#include "pch.h"
#include "NidhoggCompiler.h"

int main(int argc, char* argv[]) {
    if (argc != 3 && argc != 2) {
        std::cerr << "[-] Usage: " << argv[0] << " <input script file> <output compiled file directory>" << std::endl;
        std::cerr << "[-] Usage: " << argv[0] << " <compiled file>" << std::endl;
		return 1;
    }

	try {
		NidhoggCompiler compiler(argv[1]);

		if (argc == 3) {
			if (compiler.Compile(argv[2])) {
				std::cout << "[?] Compiling..." << std::endl;
				std::stringstream path(argv[2], std::ios_base::app | std::ios_base::out);
				path << "\\out.ndhg";

				std::cout << "[+] Compiled successfully.\n[?] Validating..." << std::endl;

				if (compiler.Validate(path.str()))
					std::cout << "[+] Output is valid." << std::endl;
				else {
					std::cerr << "[-] The output is invalid." << std::endl;
					return 1;
				}
			}
			else {
				std::cerr << "[-] Failed to compile." << std::endl;
				return 1;
			}
		}
		else {
			std::cout << "[?] Validating..." << std::endl;
			if (compiler.Validate(argv[1]))
				std::cout << "[+] Input is valid." << std::endl;
			else {
				std::cerr << "[-] The input is invalid." << std::endl;
				return 1;
			}
		}
	}
	catch (std::invalid_argument& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	catch (std::filesystem::filesystem_error& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	catch (std::fstream::failure& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}

