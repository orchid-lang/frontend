#include "lexer/lexer.hpp"
#include "parser/parser.hpp"

#include <iostream>
#include <string>
#include <fstream>

void printAST(Orchid::Compiler::Frontend::AST::Node& node, int depth = 0) {
	std::string indent(depth * 2, ' ');

	std::cout << indent << "{ type: ";

	switch (node.type) {
	case Orchid::Compiler::Frontend::AST::OPERATION:
		std::cout << "OPERATION";
		break;
	case Orchid::Compiler::Frontend::AST::IDENTIFIER:
		std::cout << "IDENTIFIER";
		break;
	case Orchid::Compiler::Frontend::AST::LITERAL:
		std::cout << "LITERAL";
		break;
	case Orchid::Compiler::Frontend::AST::VARIABLE:
		std::cout << "VARIABLE";
		break;
	case Orchid::Compiler::Frontend::AST::ROOT:
		std::cout << "ROOT";
		break;
	}

	std::cout << ", token: { text: \"" << node.token.text << "\", type: " << node.token.type << " } }" << std::endl;

	// Recursively print subnodes
	for (auto& subnode : node.getSubnodes()) {
		printAST(subnode, depth + 1);
	}
}

/// <summary>
/// Main will only be included in a direct build. 
/// When included in the full compiler this file will not be added.
/// </summary>
int main(int argc, char *argv[])
{
	std::cout << "[Warning] This is the frontend only build, this is not the full compiler!" << std::endl;

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

	std::cout << AST.getSubnodes().size() << std::endl;

	std::cout << "AST: \n";
	printAST(AST);

	return 0;
}
	