#include "pch.h"
#include "PatchParser.hpp"

bool PatchParser::ValidateArgs(std::vector<byte> data, size_t index, uint16_t argsNumber, DWORD* outOffset) {
	DWORD addedOffset = 0;
	PVOID currentArg = NULL;
	DWORD argSize = 0;
	DWORD currentIndex = 0;
	size_t dataSize = data.size();

	// Validating the opcode size.
	if (dataSize < index + 1) {
		std::cerr << "[-] Error: Invalid data size." << std::endl;
		return false;
	}
	if (argsNumber != 4) {
		std::cerr << "[-] Error: Invalid amount of arguments for the option." << std::endl;
		return false;
	}

	auto CheckArg = [&](std::vector<byte>& data, size_t dataSize, DWORD currentIndex, ArgType expectedType,
		DWORD* addedOffset) {
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
		switch (expectedType) {
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

		free(currentArg);
		*addedOffset += argSize;
	};

	bool res = CheckArg(data, dataSize, index + addedOffset, ArgType::ULong, &addedOffset) &&
	CheckArg(data, dataSize, index + addedOffset + 1, ArgType::CharPtr, &addedOffset) &&
	CheckArg(data, dataSize, index + addedOffset + 2, ArgType::CharPtr, &addedOffset) &&
	CheckArg(data, dataSize, index + addedOffset + 3, ArgType::ULong, &addedOffset);

	*outOffset += addedOffset;
	return res;
}