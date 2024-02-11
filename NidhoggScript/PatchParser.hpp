#pragma once
#include "pch.h"
#include "Parser.hpp"

class PatchParser : public Parser
{
public:
	PatchParser() {
		this->opcode = ParserOpcode::Patch;
	}
	~PatchParser() {}
	bool ValidateArgs(std::vector<byte> data, size_t index, uint16_t argsNumber, DWORD* outOffset) override;
};

