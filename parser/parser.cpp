#include "parser.hpp"
#include "../lexer/lexer.hpp"

#include <iostream>

namespace Orchid::Compiler::Frontend::Parser {
    Orchid::Compiler::Frontend::AST::Node generateAST(std::vector<Orchid::Compiler::Frontend::Lexer::Token> tokens) {
        Orchid::Compiler::Frontend::Lexer::Token current = tokens[0];
        Orchid::Compiler::Frontend::Lexer::Token lookahead = tokens[0];
        int* index = new int{-1};

        auto advance = [&]() {
			current = tokens[++*index];
            lookahead = tokens[*index+1];
		};

        advance();

        Orchid::Compiler::Frontend::AST::Node root = {
            type: Orchid::Compiler::Frontend::AST::ROOT,
            token: Orchid::Compiler::Frontend::Lexer::Token(Orchid::Compiler::Frontend::Lexer::TokenType::WHITESPACE, "[ROOT]", -1,-1,-1),
            subnodes: std::vector<Orchid::Compiler::Frontend::AST::Node>(), 
        };

        while (*index < tokens.size()) {
            if (current.type == Orchid::Compiler::Frontend::Lexer::TokenType::OPERATOR) {
                std::cout << current.text;
            }

            advance();
        }

        return root;
    }
}