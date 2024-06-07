// File id: 0 : 00000000; in parser
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

static bool operator ==(Orchid::Compiler::Frontend::AST::Node first, Orchid::Compiler::Frontend::AST::Node second)
{
    // Could also just check the token as that is unique
    // However this is just to be sure.
    return (
        first.type == second.type &&
        first.token == second.token
    );
}

namespace Orchid::Compiler::Frontend::Parser {
    Orchid::Compiler::Frontend::AST::Node generateAST(std::vector<Orchid::Compiler::Frontend::Lexer::Token> tokens);
}
