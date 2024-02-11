#include "pch.h"
#include "Parser.hpp"

bool Parser::ValidateArgs(std::vector<byte> data, size_t index, uint16_t argsNumber, DWORD* outOffset) {
	size_t dataSize = data.size();

	// Validating the opcode size.
	if (dataSize < index) {
		std::cerr << "[-] Error: Invalid data size." << std::endl;
		return false;
	}

	if (data[index] != 1) {
		std::cerr << "[-] Error: Invalid option opcode size." << std::endl;
		return false;
	}

	if (dataSize < index + 1) {
		std::cerr << "[-] Error: Invalid data size." << std::endl;
		return false;
	}

	// Check the option exists for the command type.
	Options option = (Options)data[index + 1];

	if (this->options.find(option) == this->options.end()) {
		std::cerr << "[-] Error: Option not supported for the command type." << std::endl;
		return false;
	}

	if (this->options[option].argsNumber != argsNumber) {
		std::cerr << "[-] Error: Invalid amount of arguments for the option." << std::endl;
		return false;
	}

	// opcode size + 1 (opcode).
	DWORD addedOffset = 2;
	PVOID currentArg = NULL;
	DWORD argSize = 0;
	DWORD currentIndex = 0;

	for (uint16_t i = 0; i < argsNumber; i++) {
		currentIndex = (DWORD)index + addedOffset + i;

		if (dataSize < currentIndex) {
			std::cerr << "[-] Error: Invalid data size." << std::endl;
			return false;
		}
		argSize = data[currentIndex];

		// Validating the size.
		if (argSize > dataSize - currentIndex) {
			std::cerr << "[-] Error: Invalid argument size." << std::endl;
			return false;
		}
		currentArg = (PVOID)malloc(argSize);

		if (!currentArg) {
			std::cerr << "[-] Error: Failed to allocate memory, can't validate." << std::endl;
			return false;
		}
		memset(currentArg, 0, argSize);
		memcpy(currentArg, &data[currentIndex + 1], argSize);

		// Validating the type.
		switch (this->options[option].types[i]) {
		case ArgType::ULong:
		{
			for (DWORD j = 0; j < argSize; j++) {
				if (isdigit(((char*)currentArg)[j]) == 0) {
					free(currentArg);
					std::cerr << "[-] Error: Invalid argument type." << std::endl;
					return false;
				}
			}
			break;
		}
		case ArgType::CharPtr:
		case ArgType::WCharPtr:
		{
			for (DWORD j = 0; j < argSize; j++) {
				if (!isChar(((char*)currentArg)[j])) {
					free(currentArg);
					std::cerr << "[-] Error: Invalid argument type." << std::endl;
					return false;
				}
			}
			break;
		}
		}

		// argsSize + 1 because of the size of the argSize variable.
		free(currentArg);
		addedOffset += argSize;
	}

	*outOffset += addedOffset;
	return true;
}
