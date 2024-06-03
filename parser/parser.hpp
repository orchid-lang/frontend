#pragma once
#include <vector>
#include "../lexer/lexer.hpp"

namespace Orchid::Compiler::Frontend::AST {
    enum NodeType {
        OPERATION,
        IDENTIFIER,
        LITERAL,
        VARIABLE,
        ROOT
    };

    struct Node
    {
        NodeType type;
        Orchid::Compiler::Frontend::Lexer::Token token;
        std::vector<Node> subnodes;

        Node(NodeType t, Orchid::Compiler::Frontend::Lexer::Token token, std::vector<Node> subnodes);
    };
}

namespace Orchid::Compiler::Frontend::Parser {
    Orchid::Compiler::Frontend::AST::Node generateAST(std::vector<Orchid::Compiler::Frontend::Lexer::Token> tokens);
}
