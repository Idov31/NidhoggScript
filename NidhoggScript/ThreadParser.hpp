#pragma once
#include "pch.h"
#include "Parser.hpp"

class ThreadParser : public Parser
{
public:
	ThreadParser() {
		this->opcode = ParserOpcode::Thread;

		this->options[Options::Add] = { 1, {ArgType::ULong} };
		this->options[Options::Remove] = { 1, {ArgType::ULong} };
		this->options[Options::Clear] = { 0, {} };
		this->options[Options::Hide] = { 1, {ArgType::ULong} };
		this->options[Options::Unhide] = { 1, {ArgType::ULong} };
	}
	~ThreadParser() {}
};

