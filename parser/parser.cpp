#include "parser.hpp"
#include "../lexer/lexer.hpp"

#include <memory>
#include <iostream>
#include <stdexcept>
#include <format>
#include <algorithm>

template<typename T>
bool vector_has(std::vector<T> vec, T item) {
    return std::find(vec.begin(), vec.end(), item) != vec.end();
}

namespace Orchid::Compiler::Frontend::AST {
        Node::Node(NodeType t, Orchid::Compiler::Frontend::Lexer::Token token)
            : type(t), token(token) {}

        Node::Node(NodeType t, Orchid::Compiler::Frontend::Lexer::Token token, std::vector<std::unique_ptr<Node>> subnodes)
            : type(t), token(token), subnodes(std::move(subnodes)) {}

        void Node::addSubNode(std::unique_ptr<Node> node) {
            if (!node) {
                std::cerr << "Warning: Attempt to add a null subnode\n";
                return;
            }
            this->subnodes.push_back(std::move(node));
        }

        const std::vector<std::unique_ptr<Node>>& Node::getSubnodes() const {
            return this->subnodes;
        }
}

namespace Orchid::Compiler::Frontend::Parser {
    using NodePtr = std::unique_ptr<Orchid::Compiler::Frontend::AST::Node>;

    NodePtr createNode(Orchid::Compiler::Frontend::AST::NodeType type, Orchid::Compiler::Frontend::Lexer::Token token) {
        return std::make_unique<Orchid::Compiler::Frontend::AST::Node>(type, token);
    }

    Orchid::Compiler::Frontend::AST::Node* generateAST(std::vector<Orchid::Compiler::Frontend::Lexer::Token> tokens) {
        if (tokens.empty()) {
            throw std::runtime_error("No tokens given!");
        }

        auto current = tokens.begin();
        auto lookahead = tokens.begin();
        long long index = 0;

        bool hasStarted = false;

        auto advance = [&]() {
            if (lookahead != tokens.end()) {
                ++lookahead;
                if (!hasStarted) {
                    hasStarted = true;
                    return;
                }
                ++current;
            }
		};

        auto root = createNode(Orchid::Compiler::Frontend::AST::NodeType::ROOT,
                               Orchid::Compiler::Frontend::Lexer::Token(Orchid::Compiler::Frontend::Lexer::TokenType::WHITESPACE, "[ROOT]", -1, -1, -1));

        std::vector<Orchid::Compiler::Frontend::AST::Node*> parentStack = { root.get() };

        while (lookahead != tokens.end()) {
            if (parentStack.empty()) {
                throw std::runtime_error("Please report this! \nroot popped from parent stack!\nReport here: https://github.com/orchid-lang/frontend/issues");
            }
            
            advance();

            // identifier name can't be keyword name
            if (current->type == Orchid::Compiler::Frontend::Lexer::TokenType::IDENTIFIER && vector_has(Orchid::Compiler::Frontend::Lexer::KEYWORDS, current->text)) {
                throw std::runtime_error("Please report this! \nThis error shouldn't be possible! \nReport here: https://github.com/orchid-lang/frontend/issues");
            }

            switch (current->type)
            {
            case Lexer::KEYWORD:
                // Functions
                if (current->text == "start") {
                    if ((lookahead->type != Lexer::KEYWORD || !vector_has({ "function", "lambda" }, lookahead->text)) && lookahead->text != "main") {
                        throw std::runtime_error(std::format("Loose 'start' keyword! (ln:{};cl:{},id:{})", current->line, current->column, current->index));
                    }
                    else {
                        auto functionNode = createNode(Orchid::Compiler::Frontend::AST::NodeType::OPERATION, *current);
                        parentStack.back()->addSubNode(std::move(functionNode));
                        parentStack.push_back(parentStack.back()->getSubnodes().back().get());
                    }
                }

                // TODO: fix this ?
                if (current->text == "end") {
                    if (parentStack.back() == root.get()) {
                        throw new std::runtime_error(std::format("Cannot end when not in function body! (ln:{};cl:{},id:{})", current->line, current->column, current->index));
                    }

                    parentStack.pop_back();
                    if (parentStack.empty()) {
                        throw std::runtime_error("Parent stack became empty unexpectedly.");
                    }
                }

                // Blocks
                if (current->text == "define") {
                    if (lookahead->type != Lexer::KEYWORD || lookahead->text != "as") {
                        throw std::runtime_error(std::format("'define' needs 'as'! (ln:{};cl:{},id:{})", current->line, current->column, current->index));
                    }
                }

                if (current->text == "as") {
                    if (lookahead->type != Lexer::SEPERATOR || lookahead->text != "{") {
                        throw std::runtime_error(std::format("expected opening a block with '{}'! (ln:{};cl:{},id:{})", "{", current->line, current->column, current->index));
                    }

                    auto codeBlock = createNode(Orchid::Compiler::Frontend::AST::NodeType::VARIABLE, *current);
                    parentStack.back()->addSubNode(std::move(codeBlock));
                    parentStack.push_back(parentStack.back()->getSubnodes().back().get());
                }

                // Variables
                if (current->text == "let" || current->text == "make") {
                    if (lookahead->type != Lexer::IDENTIFIER) {
                        throw std::runtime_error(std::format("No identifier found!\nDid you name your variable?\n (ln:{};cl:{},id:{})", current->line, current->column, current->index));
                    }
                }

                break;
            case Lexer::IDENTIFIER:
            {
                auto identifierNode = createNode(Orchid::Compiler::Frontend::AST::NodeType::IDENTIFIER, *current);
                parentStack.back()->addSubNode(std::move(identifierNode));
                break;
            }
            case Lexer::SEPERATOR:
                // TODO: Handle seperators
                break;
            case Lexer::STRINGLITERAL:
            case Lexer::NUMERICLITERAL:
            {
				auto literalNode = createNode(Orchid::Compiler::Frontend::AST::NodeType::LITERAL, *current);
				parentStack.back()->addSubNode(std::move(literalNode));
                break;
			}
            case Lexer::OPERATOR:
			{
				// Get the top of the parent stack.
				auto first = std::move(parentStack.back());
				parentStack.pop_back();

				// Add the current node to the parent stack
				auto OperatorNode = createNode(Orchid::Compiler::Frontend::AST::NodeType::OPERATION, *current);
				// FIXME: idk why this doesn't work, I think I'm just stupid.
				parentStack.push_back(OperatorNode);

				// add the recent top as our child
				parentStack.back()->addSubNode(std::make_unique<AST::Node>(first));
				break;
			}    
            case Lexer::COMMENT:
            case Lexer::WHITESPACE:
                break;
            default:
                throw std::runtime_error(std::format("Unknown token type for '{}' (ln:{};cl:{},id:{})", current->text, current->line, current->column, current->index));
            }

            if (parentStack.empty()) {
                throw std::runtime_error("Parent stack became empty unexpectedly.");
            }
        }

        return root.release();
    }
}
