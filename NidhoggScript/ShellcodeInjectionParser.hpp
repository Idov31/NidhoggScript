#pragma once
#include "pch.h"
#include "Parser.hpp"

class ShellcodeInjectionParser : public Parser
{
public:
	ShellcodeInjectionParser() {
		this->opcode = ParserOpcode::Shinject;

		this->options[Options::APC] = { 6,
			{ArgType::ULong, ArgType::VoidPtr, ArgType::ULong, ArgType::VoidPtr,
			ArgType::VoidPtr, ArgType::VoidPtr}
		};
		this->options[Options::Thread] = { 6,
			{ArgType::ULong, ArgType::VoidPtr, ArgType::ULong, ArgType::VoidPtr,
			ArgType::VoidPtr, ArgType::VoidPtr}
		};
	}
	~ShellcodeInjectionParser() {}
};

