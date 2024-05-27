#include "lexer.hpp"

bool is_whitespace(char c) {
	return (c == SPACE || c == TAB || c == NEWLINE || c == CR);
}

namespace Orchid::Compiler::Frontend::Lexer {
	Token::Token(TokenType t, const std::string& txt, int ln, int col, int idx)
		: type(t), text(txt), line(ln), column(col), index(idx) {}

	std::vector<Token> getTokens(std::string input) {
		std::vector<Token> foundTokens;
		State state = State::START;
		std::string currentTokenText;
		int line = 1;
		int column = 1;
		int index = 0;
		TokenType currentTokenType;

		auto pushToken = [&](TokenType type) {
			foundTokens.push_back(Token(
				type,
				currentTokenText,
				line,
				column - currentTokenText.size(),
				index - currentTokenText.size()
			));
			currentTokenText.clear();
		};

		auto advance = [&](char& c) {
			if (index >= input.size()) {
				state = State::DONE;
				return;
			}
			c = input[index++];
			column++;
			if (c == '\n') {
				line++;
				column = 0;
			}
		};

		char currentChar = '\0';
		advance(currentChar);

		while (state != State::DONE) {
			switch (state) {
			case State::START:
				if (is_whitespace(currentChar)) {
					advance(currentChar);
				}
			}
		}
	}
}