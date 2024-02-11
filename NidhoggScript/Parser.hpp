#pragma once
#include "pch.h"

enum class ArgType {
	ULong,
	CharPtr,
	WCharPtr,
	VoidPtr,
};

enum class ParserOpcode {
	Process,
	Thread,
	Module,
	Driver,
	File,
	Reg,
	Patch,
	Shinject,
	Dllinject,
	Callbacks,
	Etwti,
	Port,
	Unknown = 0xFF
};

enum class Options {
	Add, Remove,
	Clear, Hide,
	Unhide, Elevate,
	Signature, APC,
	Thread,
	Enable = 0, Disable = 1,
	Restore = 0
};

typedef struct _ArgMetadata {
	uint16_t argsNumber;
	std::vector<ArgType> types;
} ArgMetadata;

class Parser {
protected:
	ParserOpcode opcode;
	std::unordered_map<Options, ArgMetadata> options;

	constexpr bool isChar(char c) {
		return c >= ' ' && c <= '~';
	}
public:
	Parser() { opcode = ParserOpcode::Unknown; }
	virtual ~Parser() {}

	virtual bool ValidateArgs(std::vector<byte> data, size_t index, uint16_t argsNumber, DWORD* outOffset);
	uint16_t GetOpcode() { return (uint16_t)opcode; }
};