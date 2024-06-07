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

        Orchid::Compiler::Frontend::AST::Node currentParent = root;

        // Loop over all tokens but skipping the last
        while (*index < (signed)tokens.size()) {
            advance();

            // identifier name can't be keyword name
            if (current.type == Orchid::Compiler::Frontend::Lexer::TokenType::IDENTIFIER && vector_has(Orchid::Compiler::Frontend::Lexer::KEYWORDS, current.text)) {
                throw std::runtime_error("Please report this! \nThis error shouldn't be possible! \nReport here: https://github.com/orchid-lang/frontend/issues");
            }

            switch (current.type)
            {
            case Lexer::KEYWORD:
                if (current.text == "start") {
                    if (lookahead.type != Lexer::KEYWORD && lookahead.text != "main") {
                        throw std::runtime_error(std::format("Loose 'start' keyword! (ln:{};cl:{},id:{})", current.line, current.column, current.index));
                    } else {
                        Orchid::Compiler::Frontend::AST::Node functionNode(Orchid::Compiler::Frontend::AST::NodeType::OPERATION, current, {});
                        currentParent = functionNode;
                    }
                }

                if (current.text == "end") {
                    if (currentParent == root) {
                        throw new std::runtime_error(std::format("Cannot end when not in function body! (ln:{};cl:{},id:{})", current.line, current.column, current.index));
                    }
                }
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