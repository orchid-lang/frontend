#pragma once
#include <string>
#include <vector>

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
		LITERAL,
		COMMENT,
		WHITESPACE,
		TYPE,
		OPERATOR,
	};

	enum LiteralType {
		NOTALITERAL,
		STRINGLITERAL,
		NUMERICLITERAL,
		BOOLEANLITERAL,
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

		LiteralType literal_type = LiteralType::NOTALITERAL;
		TypeType type_type = TypeType::NOTATYPE;

		Token(TokenType t, const std::string& txt, int ln, int col, int idx);
	};

	enum State {
		START,
		IDENTIFIER,
		NUMBER,
		STRING,
		COMMENT,
		DONE
	};

	std::vector<Token> getTokens(std::string input);
}
