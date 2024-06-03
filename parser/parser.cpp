#include "parser.hpp"
#include "../lexer/lexer.hpp"

#include <iostream>

Orchid::Compiler::Frontend::AST::Node::Node(NodeType t, Orchid::Compiler::Frontend::Lexer::Token token, std::vector<Node> subnodes)
    : type(t), token(token), subnodes(subnodes) {}

namespace Orchid::Compiler::Frontend::Parser {
    Orchid::Compiler::Frontend::AST::Node generateAST(std::vector<Orchid::Compiler::Frontend::Lexer::Token> tokens) {
        Orchid::Compiler::Frontend::Lexer::Token current = tokens[0];
        Orchid::Compiler::Frontend::Lexer::Token lookahead = tokens[0];
        long long* index = new long long{-1};

        auto advance = [&]() {
			current = lookahead;
            if (++*index < (signed)tokens.size()) {
                lookahead = tokens[*index];
            }
		};

        advance();

        Orchid::Compiler::Frontend::AST::Node root = Orchid::Compiler::Frontend::AST::Node(
            Orchid::Compiler::Frontend::AST::ROOT,
            Orchid::Compiler::Frontend::Lexer::Token(Orchid::Compiler::Frontend::Lexer::TokenType::WHITESPACE, "[ROOT]", -1, -1, -1),
            std::vector<Orchid::Compiler::Frontend::AST::Node>()
            );

        // Loop over all tokens but skipping the last
        while (*index < (signed)tokens.size()) {
            if (current.type == Orchid::Compiler::Frontend::Lexer::TokenType::OPERATOR) {
                std::cout << current.text;
            }

            advance();
        }

        delete index;

        return root;
    }
}