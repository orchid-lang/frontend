#include "../lexer/lexer.hpp"
#include <iostream>
#include <vector>

#define END '\n'

using namespace Orchid::Compiler::Frontend::Lexer;

bool testWhitespace() {
	std::string input = "	\t\n" + END;
	auto tokens = getTokens(input);
	return tokens.empty();
}

bool testIdentifiers() {
	std::string input = "foo bar" + END;
	auto tokens = getTokens(input);
	return (
		tokens.size() == 2 &&
		tokens[0].type == TokenType::IDENTIFIER && tokens[0].text == "foo" &&
		tokens[1].type == TokenType::IDENTIFIER && tokens[1].text == "bar"
		);
}

bool testNumbers() {
	std::string input = "123 456 890" + END;
	auto tokens = getTokens(input);
	return (
		tokens.size() == 3 &&
		tokens[0].type == TokenType::NUMERICLITERAL && tokens[0].text == "123" &&
		tokens[1].type == TokenType::NUMERICLITERAL && tokens[1].text == "456" &&
		tokens[2].type == TokenType::NUMERICLITERAL && tokens[2].text == "890"
		);
}

int main() {
	std::vector<bool> tests = {
		testWhitespace(),
		testIdentifiers(),
		testNumbers(),
	};
	std::vector<std::string> testNames = {
		"Whitespace",
		"Identifiers",
		"Numeric Literals"
	};

	bool failed = false;
	for (int i = 0; i < tests.size(); i++)
	{
		if (!tests[i]) {
			std::cout << "\t[FAILED] test: " << testNames[i] << std::endl;
			failed = true;
		}
		else {
			std::cout << "\t[PASSED] test: " << testNames[i] << std::endl;
		}
	}
	
	if (failed) {
		return 1;
	}
	return 0;
}
