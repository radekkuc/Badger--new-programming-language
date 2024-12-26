#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include "lexer.h"

struct ASTNode {
    std::string type;
    std::string value;
    ASTNode* left;
    ASTNode* right;

    ASTNode(std::string type, std::string value, ASTNode* left = nullptr, ASTNode* right = nullptr)
        : type(type), value(value), left(left), right(right) {
    }
};

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    ASTNode* parse();
    void printNode(ASTNode* node, int depth = 0);

private:
    const std::vector<Token> tokens;
    size_t current_position;

    Token advance();
    Token peek();
    bool match(const std::string& type);

    ASTNode* parse_program();
    ASTNode* parse_statement();
    ASTNode* parse_expression();
    ASTNode* parse_term();
    ASTNode* parse_factor();
};

#endif
