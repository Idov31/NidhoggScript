#pragma once
#include "pch.h"

class NidhoggScriptValidator
{
private:
	std::string script;
public:
	NidhoggScriptValidator(std::string script);
	~NidhoggScriptValidator() {};
	bool Validator();
};

