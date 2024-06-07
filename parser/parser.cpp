// File id: 1 : 00000001; in parser
#include "parser.hpp"
#include "../lexer/lexer.hpp"

#include <iostream>
#include <stdexcept>

template<typename T>
bool vector_has(std::vector<T> vec, T item) {
    return std::find(vec.begin(), vec.end(), item) != vec.end();
}

Orchid::Compiler::Frontend::AST::Node::Node(NodeType t, Orchid::Compiler::Frontend::Lexer::Token token, std::vector<Node> subnodes)
    : type(t), token(token), subnodes(subnodes) {}

namespace Orchid::Compiler::Frontend::Parser {
    Orchid::Compiler::Frontend::AST::Node generateAST(std::vector<Orchid::Compiler::Frontend::Lexer::Token> tokens) {
        Orchid::Compiler::Frontend::Lexer::Token current = tokens[0];
        Orchid::Compiler::Frontend::Lexer::Token lookahead = tokens[0];
        long long* index = new long long{0};

        auto advance = [&]() {
			current = lookahead;
            if (++*index < (signed)tokens.size()) {
                lookahead = tokens[*index];
            }
		};

        Orchid::Compiler::Frontend::AST::Node root = Orchid::Compiler::Frontend::AST::Node(
            Orchid::Compiler::Frontend::AST::ROOT,
            Orchid::Compiler::Frontend::Lexer::Token(Orchid::Compiler::Frontend::Lexer::TokenType::WHITESPACE, "[ROOT]", -1, -1, -1),
            std::vector<Orchid::Compiler::Frontend::AST::Node>()
            );

        // Loop over all tokens but skipping the last
        while (*index < (signed)tokens.size()) {
            advance();

            // identifier name can't be keyword name
            if (current.type == Orchid::Compiler::Frontend::Lexer::TokenType::IDENTIFIER && vector_has(Orchid::Compiler::Frontend::Lexer::KEYWORDS, current.text)) {
                throw std::runtime_error("Please report this! \nThis error shouldn't be possible! \nReport here: https://github.com/orchid-lang/frontend/issues \nError code: 302901");
            }

            switch (current.type)
            {
            case Lexer::KEYWORD:
                // Handle keywords
                break;
            case Lexer::IDENTIFIER:
                // Handle identifiers
                break;
            case Lexer::SEPERATOR:
                // Handle seperators
                break;
            case Lexer::STRINGLITERAL:
            case Lexer::NUMERICLITERAL:
                // Handle literals
                break;
            case Lexer::OPERATOR:
                // Handle operators
                break;
            case Lexer::COMMENT:
            case Lexer::WHITESPACE:
                break;
            default:
                throw std::runtime_error(std::format("Unknown token type for '{}' (ln:{};cl:{},id:{})", current.text, current.line, current.column, current.index));
            }
        }

        delete index;

        return root;
    }
}