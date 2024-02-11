#include "pch.h"
#include "NidhoggAssembler.h"

int main(int argc, char* argv[]) {
    if (argc != 3 && argc != 2) {
        std::cerr << "[-] Usage: " << argv[0] << " <input script file> <output assembled file directory>" << std::endl;
        std::cerr << "[-] Usage: " << argv[0] << " <assembled file>" << std::endl;
		return 1;
    }

	try {
		NidhoggAssembler assembler(argv[1]);

		if (argc == 3) {
			if (assembler.Assemble(argv[2])) {
				std::cout << "[?] Assembling..." << std::endl;
				std::stringstream path(argv[2], std::ios_base::app | std::ios_base::out);
				path << "\\out.ndhg";

				std::cout << "[+] Assembled successfully.\n[?] Validating..." << std::endl;

				if (assembler.Validate(path.str()))
					std::cout << "[+] Output is valid." << std::endl;
				else {
					std::cerr << "[-] The output is invalid." << std::endl;
					return 1;
				}
			}
			else {
				std::cerr << "[-] Failed to assemble." << std::endl;
				return 1;
			}
		}
		else {
			std::cout << "[?] Validating..." << std::endl;
			if (assembler.Validate(argv[1]))
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

