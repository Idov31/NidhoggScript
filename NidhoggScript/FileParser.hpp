#pragma once
#include "pch.h"
#include "Parser.hpp"

class FileParser : public Parser
{
public:
	FileParser() {
		this->opcode = ParserOpcode::File;

		this->options[Options::Add] = { 1, {ArgType::WCharPtr} };
		this->options[Options::Remove] = { 1, {ArgType::WCharPtr} };
		this->options[Options::Clear] = { 0, {} };
	}
	~FileParser() {}
};

