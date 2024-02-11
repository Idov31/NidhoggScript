#include "pch.h"
#include "NidhoggAssembler.h"

NidhoggAssembler::NidhoggAssembler(std::string filePath) {
	std::string token;

	if (filePath.empty())
		throw std::invalid_argument("[-] Error: File path cannot be empty.");

	if (!std::filesystem::exists(filePath))
		throw std::filesystem::filesystem_error("[-] Error: File does not exist.", std::error_code());

	size_t length = std::filesystem::file_size(filePath);

	if (length == 0)
		throw std::fstream::failure("[-] Error: File cannot be empty.");

	std::ifstream fileStream(filePath);

	if (!fileStream.is_open())
		throw std::fstream::failure("[-] Error: Failed to open file.");

	std::string data((std::istreambuf_iterator<char>(fileStream)),
		std::istreambuf_iterator<char>());

	size_t pos = 0;

	while ((pos = data.find("\n")) != std::string::npos) {
		token = data.substr(0, pos);
		this->commands.push_back(token);
		data.erase(0, pos + 1);
	}

	if (!data.empty())
		this->commands.push_back(data);

	// Initialize parsers.
	this->parsersMap = {
		{"process", new ProcessParser()},
		{"thread", new ThreadParser()},
		{"module", new ModuleParser()},
		{"driver", new DriverParser()},
		{"file", new FileParser()},
		{"reg", new RegParser()},
		{"patch", new PatchParser()},
		{"shinject", new ShellcodeInjectionParser()},
		{"dllinject", new DllInjectionParser()},
		{"callbacks", new CallbacksParser()},
		{"etwti", new EtwTiParser()},
		{"port", new PortParser()}
	};
}

NidhoggAssembler::~NidhoggAssembler() {
	for (auto& parser : this->parsersMap)
		delete parser.second;
}

bool NidhoggAssembler::Assemble(std::string outputPath) {
	std::vector<byte> assembledBytes;
	std::vector<std::string> args;
	uint16_t commandType = 0;
	DWORD startIndex = 0;
	bool assembled = true;
	assembledBytes.insert(assembledBytes.end(), SIGNATURE, SIGNATURE + sizeof(SIGNATURE));

	// Push the size of the commands.
	auto PushInt = [&](std::vector<byte>& assembledBytes, size_t size) {
		size_t intToPush = size;

		if (intToPush == 0)
			assembledBytes.push_back(0);
		else {
			if (intToPush > 0xFF) {
				while (intToPush != 0) {
					assembledBytes.push_back(intToPush & 0xFF);
					intToPush >>= 8;
				}
			}
			else
				assembledBytes.push_back(intToPush);
		}
	};

	size_t commandsSize = this->commands.size();
	PushInt(assembledBytes, commandsSize);

	for (std::string& command : this->commands) {
		// Push the command.
		commandType = GetCommandType(command);

		if (commandType == INVALID_COMMAND_TYPE) {
			assembled = false;
			break;
		}

		PushInt(assembledBytes, commandType);

		// Push the args.
		args = GetArgs(command);

		if (args.size() > 0) {
			PushInt(assembledBytes, args.size());

			if ((ParserOpcode)commandType != ParserOpcode::Patch) {
				std::string option = args[0];

				if (this->options.find(option) == this->options.end()) {
					assembled = false;
					break;
				}

				// Pushing the option's opcode.
				PushInt(assembledBytes, 1);
				PushInt(assembledBytes, this->options[option]);
				startIndex = 1;
			}
			
			// Pushing the rest of the args.
			for (DWORD i = startIndex; i < args.size(); i++) {
				PushInt(assembledBytes, args[i].size());
				assembledBytes.insert(assembledBytes.end(), args[i].begin(), args[i].end());
			}
		}
		else {
			assembledBytes.push_back(0);
			assembledBytes.push_back(0);
		}
	}

	if (assembled) {
		std::ofstream fileStream;

		if (outputPath.empty())
			outputPath = std::filesystem::current_path().string();
		std::stringstream path(outputPath, std::ios_base::app | std::ios_base::out);
		path << "\\out.ndhg";
		fileStream.open(path.str(), std::ios::out | std::ios::binary);
		fileStream.write((char*)assembledBytes.data(), assembledBytes.size());
	}
	return assembled;
}

bool NidhoggAssembler::Validate(std::string assembledFilePath) {
	DWORD offset = 0;

	if (assembledFilePath.empty()) {
		std::cerr << "[-] Error: File path cannot be empty." << std::endl;
		return false;
	}

	if (!std::filesystem::exists(assembledFilePath)) {
		std::cerr << "[-] Error: File does not exist." << std::endl;
		return false;
	}

	size_t length = std::filesystem::file_size(assembledFilePath);

	if (length < MINIMUM_FILE_SIZE) {
		std::cerr << "[-] Error: Invalid file size." << std::endl;
		return false;
	}

	std::vector<byte> data(length);
	std::ifstream fileStream(assembledFilePath, std::ios::binary);
	fileStream.read((char*)data.data(), length);

	// Checking signature.
	if (memcmp(data.data(), SIGNATURE, sizeof(SIGNATURE)) != 0) {
		std::cerr << "[-] Error: Invalid file signature." << std::endl;
		return false;
	}

	// Checking commands size.
	size_t commandsSize = data[4];

	if (commandsSize == 0) {
		std::cerr << "[-] Error: Invalid commands size." << std::endl;
		return false;
	}

	for (DWORD i = 0; i < commandsSize; i++) {
		if (!ValidateCommand(data, FIRST_COMMAND_OFFSET + offset + i, &offset))
			return false;
	}

	return true;
}

bool NidhoggAssembler::ValidateCommand(std::vector<byte> data, size_t index, DWORD* outOffset) {
	uint16_t commandType = data[index];

	if (reverseOpcodeMap.find(commandType) == reverseOpcodeMap.end()) {
		std::cerr << "[-] Error: Unknown command type." << std::endl;
		return false;
	}
	*outOffset += 1;
	uint16_t argsNumber = data[index + 1];

	if (argsNumber > 0) {
		*outOffset += 1;
		argsNumber = (ParserOpcode)commandType == ParserOpcode::Patch ? argsNumber : argsNumber - 1;

		// index + 2 because of the args number and the command type.
		return parsersMap[reverseOpcodeMap[commandType]]->ValidateArgs(data, index + 2, argsNumber,
			outOffset);
	}

	return false;
}

uint16_t NidhoggAssembler::GetCommandType(std::string command) {
	size_t pos = command.find(" ");
	std::string commandName = command.substr(0, pos);

	if (this->parsersMap.find(commandName) == this->parsersMap.end())
		return INVALID_COMMAND_TYPE;

	return this->parsersMap[commandName]->GetOpcode();
}

std::vector<std::string> NidhoggAssembler::GetArgs(std::string command) {
	bool isString = false;
	std::string token;
	std::string strToken;
	size_t quotesPos = 0;
	std::vector<std::string> args;

	// Remove the command name from the string.
	size_t pos = command.find(" ");
	command.erase(0, pos + 1);

	// Parse the arguments.
	while ((pos = command.find(" ")) != std::string::npos) {
		token = command.substr(0, pos);
		quotesPos = token.find('"');

		// If it is a string with spaces, include everything until the end of the string (marked by ").
		if (quotesPos != std::string::npos) {
			if (token[0] == '"') {
				isString = true;
				token.erase(std::remove(token.begin(), token.end(), '"'), token.end());
				strToken = token;
			}
			if (token[token.length() - 1] == '"') {
				if (strToken != token) {
					token.erase(std::remove(token.begin(), token.end(), '"'), token.end());
					strToken += token;
				}
				isString = false;
				args.push_back(strToken);
				strToken = "";
			}
		}
		else if (isString)
			strToken += " " + token;
		else
			args.push_back(token);
		command.erase(0, pos + 1);
	}

	if (!command.empty()) {
		if (isString) {
			if (command[command.length() - 1] == '"') {
				command.erase(std::remove(command.begin(), command.end(), '"'), command.end());
				isString = false;
				strToken += " " + command;
				args.push_back(strToken);
			}
		}
		else 
			args.push_back(command);
	}

	return args;
}
