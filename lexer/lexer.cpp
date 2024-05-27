#include "lexer.hpp"

bool is_whitespace(char c) {
	return (c == SPACE || c == TAB || c == NEWLINE || c == CR);
}

bool is_letter(char c) {
	return std::isalpha(c) || c == '_';
}

bool is_digit(char c) {
	return std::isdigit(c);
}

bool is_symbol(char c) {
	return c == '{' || c == '}' || c == '(' || c == ')' || c == ';' || c == ','
		|| c == ':' || c == '=' || c == '+' || c == '-' || c == '*' || c == '/';
}

/// <summary>
/// See if a symbol is a seperator or an operator
/// </summary>
/// <param name="c">The character to check</param>
/// <returns>True for seperator. False for operator</returns>
bool is_sep_or_op(char c) {
	return c == '{' || c == '}' || c == '(' || c == ')' || c == ';' || c == ',' || c == ':';
}

template<typename T>
bool vector_has(std::vector<T> vec, T item) {
	return std::find(vec.begin(), vec.end(), item) != vec.end();
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
				else if (is_letter(currentChar)) {
					state = State::IDENTIFIER;
					currentTokenText += currentChar;
					advance(currentChar);
				}
				else if (is_digit(currentChar)) {
					state = State::NUMBER;
					currentTokenText += currentChar;
					advance(currentChar);
				}
				else if (currentChar == '"') {
					state = State::STRING;
					currentTokenText += currentChar;
					advance(currentChar);
				}
				// index starts at 1 so will be one ahead.
				else if (currentChar == '/' && input[index] == '/') {
					state = State::COMMENT;
					advance(currentChar);
					advance(currentChar);	// Skip both /-es
				}
				else {
					currentTokenText += currentChar;

					if (is_symbol(currentChar)) {
						if (is_sep_or_op(currentChar)) {
							pushToken(TokenType::SEPERATOR);
						}
						else {
							pushToken(TokenType::OPERATOR);
						}
					}
					else if (currentChar == '\0') {
						state == State::DONE;
					}
					else {
						throw std::runtime_error(std::format("Unexpected token: {} (ln:{};col:{};idx:{})", std::string(1, currentChar), line, column, index));
					}

					advance(currentChar);
				}

				break;	// State::START

			case State::IDENTIFIER:
				if (is_letter(currentChar) || is_digit(currentChar)) {
					currentTokenText += currentChar;
					advance(currentChar);
				}
				else {
					if (vector_has(KEYWORDS, currentTokenText)) {
						pushToken(TokenType::KEYWORD);
					}
					else {
						pushToken(TokenType::IDENTIFIER);
					}
					state = State::START;
				}

				break;	// State::IDENTIFIER
			}
		}
	}
}