#pragma once
#include "pch.h"
#include "Parser.hpp"

class EtwTiParser : public Parser
{
public:
	EtwTiParser() {
		this->opcode = ParserOpcode::Etwti;

		this->options[Options::Enable] = { 0, {} };
		this->options[Options::Disable] = { 0, {} };
	}
	~EtwTiParser() {}
};

