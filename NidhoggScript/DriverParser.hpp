#pragma once
#include "pch.h"
#include "Parser.hpp"

class DriverParser : public Parser
{
public:
	DriverParser() {
		this->opcode = ParserOpcode::Driver;

		this->options[Options::Hide] = { 1, {ArgType::WCharPtr} };
		this->options[Options::Unhide] = { 1, {ArgType::WCharPtr} };
	}
	~DriverParser() {}
};

