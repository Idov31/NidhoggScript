#pragma once
#include "pch.h"
#include "Parser.hpp"

class ProcessParser : public Parser
{
public:
	ProcessParser() {
		this->opcode = ParserOpcode::Process;

		this->options[Options::Add] = { 1, {ArgType::ULong} };
		this->options[Options::Remove] = { 1, {ArgType::ULong} };
		this->options[Options::Clear] = { 0, {} };
		this->options[Options::Hide] = { 1, {ArgType::ULong} };
		this->options[Options::Unhide] = { 1, {ArgType::ULong} };
		this->options[Options::Elevate] = { 1, {ArgType::ULong} };
		this->options[Options::Signature] = { 3, {ArgType::ULong, ArgType::ULong, ArgType::ULong} };
	}
	~ProcessParser() {}
};

