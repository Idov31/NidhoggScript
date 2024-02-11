#pragma once
#include "pch.h"
#include "Parser.hpp"

class RegParser : public Parser
{
public:
	RegParser() {
		this->opcode = ParserOpcode::Reg;

		this->options[Options::Add] = { 2, {ArgType::WCharPtr, ArgType::WCharPtr} };
		this->options[Options::Remove] = { 2, {ArgType::WCharPtr, ArgType::WCharPtr} };
		this->options[Options::Clear] = { 0, {} };
		this->options[Options::Hide] = { 2, {ArgType::WCharPtr, ArgType::WCharPtr} };
		this->options[Options::Unhide] = { 2, {ArgType::WCharPtr, ArgType::WCharPtr} };
	}
	~RegParser() {}
};

