#include "pch.h"
#include "ProcessParser.h"

ProcessParser::ProcessParser() {
	this->options[Options::Add] = { 1, {TypeULong} };
	this->options[Options::Remove] = { 1, {TypeULong} };
	this->options[Options::Clear] = { 0, {} };
	this->options[Options::Hide] = { 1, {TypeULong} };
	this->options[Options::Unhide] = { 1, {TypeULong} };
	this->options[Options::Elevate] = { 1, {TypeULong} };
	this->options[Options::Signature] = { 3, {TypeULong, TypeULong, TypeULong} };
}