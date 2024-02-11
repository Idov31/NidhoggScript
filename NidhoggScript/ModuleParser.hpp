#pragma once
#include "pch.h"
#include "Parser.hpp"

class ModuleParser : public Parser
{
public:
	ModuleParser() {
		this->opcode = ParserOpcode::Module;

		this->options[Options::Hide] = { 1, {ArgType::WCharPtr} };
	}
	~ModuleParser() {}
};

