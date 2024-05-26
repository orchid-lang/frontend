#include "lexer.hpp"

namespace Orchid::Compiler::Frontend::Lexer {
	Token::Token(TokenType t, const std::string& txt, int ln, int col, int idx)
		: type(t), text(txt), line(ln), column(col), index(idx) {}
}