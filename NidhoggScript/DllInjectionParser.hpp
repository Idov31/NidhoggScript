#pragma once
#include "pch.h"
#include "Parser.hpp"

class DllInjectionParser : public Parser
{
public:
	DllInjectionParser() {
		this->opcode = ParserOpcode::Dllinject;

		this->options[Options::APC] = {2, {ArgType::ULong, ArgType::CharPtr}};
		this->options[Options::Thread] = { 2, {ArgType::ULong, ArgType::CharPtr} };
	}
	~DllInjectionParser() {}
};

