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

Orchid::Compiler::Frontend::AST::Node::Node(NodeType t, Orchid::Compiler::Frontend::Lexer::Token token)
    : type(t), token(token) {
    this->subnodes = {};
}

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

        Orchid::Compiler::Frontend::AST::Node* currentParent = &root;

        std::vector<AST::Node*> parentStack = { &root };

        // Loop over all tokens but skipping the last
        while (*index < (signed)tokens.size()) {
            if (parentStack.empty()) {
                throw std::runtime_error("Please report this! \nroot popped from parent stack!\nReport here: https://github.com/orchid-lang/frontend/issues");
            }

            if (!vector_has(parentStack, currentParent)) {
                throw std::runtime_error("Please report this! \nCurrent parent is not in parent stack!\nReport here: https://github.com/orchid-lang/frontend/issues");
            }
            
            currentParent = parentStack.back();

            advance();

            // identifier name can't be keyword name
            if (current.type == Orchid::Compiler::Frontend::Lexer::TokenType::IDENTIFIER && vector_has(Orchid::Compiler::Frontend::Lexer::KEYWORDS, current.text)) {
                throw std::runtime_error("Please report this! \nThis error shouldn't be possible! \nReport here: https://github.com/orchid-lang/frontend/issues");
            }

            switch (current.type)
            {
            case Lexer::KEYWORD:
                // Functions
                if (current.text == "start") {
                    if ((lookahead.type != Lexer::KEYWORD || !vector_has({"function", "lambda"}, lookahead.text)) && lookahead.text != "main") {
                        throw std::runtime_error(std::format("Loose 'start' keyword! (ln:{};cl:{},id:{})", current.line, current.column, current.index));
                    } else {
                        Orchid::Compiler::Frontend::AST::Node functionNode(Orchid::Compiler::Frontend::AST::NodeType::OPERATION, current);
                        parentStack.push_back(&functionNode);
                    }
                }

                // TODO: fix this
                if (current.text == "end") {
                    if (*currentParent == root) {
                        throw new std::runtime_error(std::format("Cannot end when not in function body! (ln:{};cl:{},id:{})", current.line, current.column, current.index));
                    }
                    parentStack.pop_back();
                }

                // Blocks
                if (current.text == "define") {
                    if (lookahead.type != Lexer::KEYWORD || lookahead.text != "as") {
                        throw std::runtime_error(std::format("'define' needs 'as'! (ln:{};cl:{},id:{})", current.line, current.column, current.index));
                    }
                }

                if (current.text == "as") {
                    if (lookahead.type != Lexer::SEPERATOR || lookahead.text != "{") {
                        throw std::runtime_error(std::format("expected opening a block with '{}'! (ln:{};cl:{},id:{})", "{", current.line, current.column, current.index));
                    }
                    // Create node for code block
                    Orchid::Compiler::Frontend::AST::Node codeBlock(Orchid::Compiler::Frontend::AST::NodeType::VARIABLE, current);
                    currentParent->addSubNode(codeBlock);
                    parentStack.push_back(&codeBlock);
                }

                // Variables
                if (current.text == "let" || current.text == "make") {
                    if (lookahead.type != Lexer::IDENTIFIER) {
                        throw std::runtime_error(std::format("No identifier found!\nDid you name your variable?\n (ln:{};cl:{},id:{})", current.line, current.column, current.index));
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