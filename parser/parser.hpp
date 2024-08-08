// File id: 0 : 00000000; in parser
#pragma once
#include <vector>
#include <memory>
#include "../lexer/lexer.hpp"

namespace Orchid::Compiler::Frontend::AST {
    enum NodeType {
        OPERATION,
        IDENTIFIER,
        LITERAL,
        VARIABLE,
        ROOT
    };

    class Node
    {
    public:
        Node(NodeType t, Orchid::Compiler::Frontend::Lexer::Token token);
        Node(NodeType t, Orchid::Compiler::Frontend::Lexer::Token token, std::vector<std::unique_ptr<Node>> subnodes);

        virtual ~Node() = default;

        Node(const Node&) = delete;
        Node(Node&&) = default;
        Node& operator=(Node&&) = default;
        Node& operator=(const Node&) = delete;

        void addSubNode(std::unique_ptr<Node> node);
        const std::vector<std::unique_ptr<Node>>& getSubnodes() const;

        NodeType getType() { return this->type; };
        Orchid::Compiler::Frontend::Lexer::Token getToken() { return this->token; };
    private:
        std::vector<std::unique_ptr<Node>> subnodes;
        NodeType type;
        Orchid::Compiler::Frontend::Lexer::Token token;
    };
}

static bool operator ==(Orchid::Compiler::Frontend::AST::Node first, Orchid::Compiler::Frontend::AST::Node second)
{
    // Could also just check the token as that is unique
    // However this is just to be sure.
    return (
        first.getType() == second.getType() &&
        first.getToken() == second.getToken()
    );
}

namespace Orchid::Compiler::Frontend::Parser {
    Orchid::Compiler::Frontend::AST::Node* generateAST(std::vector<Orchid::Compiler::Frontend::Lexer::Token> tokens);
}
