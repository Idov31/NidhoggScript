#pragma once
#include "pch.h"
#include "Parser.hpp"

class CallbacksParser : public Parser
{
public:
	CallbacksParser() {
		this->opcode = ParserOpcode::Callbacks;

		this->options[Options::Enable] = { 2, {ArgType::ULong, ArgType::ULong} };
		this->options[Options::Disable] = { 2, {ArgType::ULong, ArgType::ULong} };
	}
	~CallbacksParser() {}
};

