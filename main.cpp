#include "lexer/lexer.hpp"

#include <iostream>
#include <string>
#include <fstream>

/// <summary>
/// Main will only be included in a direct build. 
/// When included in the full compiler this file will not be added.
/// </summary>
int main(int argc, char *argv[])
{
	std::cout << "[Warning] This is the frontend only build, this is not the full compiler!" << std::endl;

	std::string infile;

	if (argc < 3) {
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

	return 0;
}
	