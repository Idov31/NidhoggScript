#pragma once
#include "pch.h"
#include "Parser.hpp"

class PortParser : public Parser
{
public:
	PortParser() {
		this->opcode = ParserOpcode::Port;

		this->options[Options::Clear] = { 0, {} };
		this->options[Options::Hide] = { 3, {ArgType::ULong, ArgType::ULong, ArgType::ULong} };
		this->options[Options::Unhide] = { 3, {ArgType::ULong, ArgType::ULong, ArgType::ULong} };
	}
	~PortParser() {}
};

