#include "lexer/lexer.hpp"
#include "parser/parser.hpp"

#include <iostream>
#include <string>
#include <fstream>

#if defined(_WIN32)
#define PLATFORM "win"
#elif defined(__linux__)
#define PLATFORM "linux"
#elif defined(__FreeBSD__)
#define PLATFORM "freebsd"
#elif defined(__APPLE__)
#define PLATFORM "mac"
#elif defined(__ANDROID__)
#define PLATFORM "andrd"
#endif

/// <summary>
/// Main will only be included in a direct build. 
/// When included in the full compiler this file will not be added.
/// </summary>
int main(int argc, char *argv[])
{
	std::cout << "[Warning] This is the frontend only build, this is not the full compiler!" << std::endl;

	if (PLATFORM == "win") {
		std::cout << "(testing file is likely located at \"../../../test.orh\")" << std::endl;
	}
	else if (PLATFORM == "linux") {
		std::cout << "(testing file is likely located at \"../test.orh\")" << std::endl;
	}
	else if (PLATFORM == "freebsd" || PLATFORM == "mac") {
		std::cout << "(How are you even here, you are not in the cmake file)" << std::endl;
		std::cout << "(testing file is likely located at \"../test.orh\")" << std::endl;
	}
	else if (PLATFORM == "andrd") {
		std::cout << "(How are you even here, you are not in the cmake file)" << std::endl;
		std::cout << "(I don't even know how you got here on android)" << std::endl;
		std::cout << "(testing file is likely located at \"../test.orh\")" << std::endl;
	}

	std::string infile;

	if (argc < 2) {
		std::cout << "What file to use as input file? ";
		std::cin >> infile;
	}
	else {	
		infile = argv[1];
	}

	std::cout << "Reading " << infile << "\n";

	// Just output a list of tokens for now
	std::ifstream file(infile);

	if (!file.is_open()) {
		std::cout << "Failed to open input file!" << std::endl;
		return 1;
	}

	std::string content;
	std::string line;
	while (std::getline(file, line)) {
		content += line + "\n";
	}

	file.close();

	auto tokens = Orchid::Compiler::Frontend::Lexer::getTokens(content);

	std::cout << "tokens: [\n";
	for (auto i : tokens)
		std::cout << "\t{ text: \"" << i.text << "\", type: " << i.type << " },\n";
	std::cout << "]" << std::endl;

	auto AST = Orchid::Compiler::Frontend::Parser::generateAST(tokens);

	return 0;
}
	