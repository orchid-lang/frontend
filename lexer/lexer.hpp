// File id: 0 : 00000000; in lexer
#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <format>
#include <algorithm>

// ' ': 32; '\t': 9; '\n': 10; '\r': 13
#define SPACE 32
#define TAB 9
#define NEWLINE 10
#define CR 13

namespace Orchid::Compiler::Frontend::Lexer {
	enum TokenType {
		KEYWORD,
		IDENTIFIER,
		SEPERATOR,
		STRINGLITERAL,
		NUMERICLITERAL,
		COMMENT,
		WHITESPACE,
		TYPE,
		OPERATOR,
	};

	enum TypeType {
		NOTATYPE,
		INT,
		STRING,
		BOOL,
	};

	struct Token {
		TokenType type;

		std::string text;
		
		int line;
		int column;
		int index;

		TypeType type_type = TypeType::NOTATYPE;

		Token(TokenType t, const std::string& txt, int ln, int col, int idx);
	};

	enum class State {
		START,
		IDENTIFIER,
		NUMBER,
		STRING,
		COMMENT,
		DONE
	};

	std::vector<Token> getTokens(std::string input);

	inline std::vector<std::string> KEYWORDS = { "start", "function", "define", "as", "takes", "gives", "let", "make", "return", "print", "end", "if", "typecheck", "then", "catch" };
}

static bool operator ==(Orchid::Compiler::Frontend::Lexer::Token first, Orchid::Compiler::Frontend::Lexer::Token second)
{
	// Most of these are unnecesairy because we could just compare the index as that must be unique.
	// However just as a sanity check these will stay here
	return (
		first.line == second.line &&
		first.column == second.column &&
		first.index == second.index &&
		first.text == second.text
	);
}
