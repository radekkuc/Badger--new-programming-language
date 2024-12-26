#include "parser.h"
#include <iostream>
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), current_position(0) {}

ASTNode* Parser::parse() {
    return parse_program();
}

ASTNode* Parser::parse_program() {
    ASTNode* result = nullptr;

    while (peek().type != "EOF") {
        ASTNode* stmt = parse_statement();

        if (!match("SEMICOLON")) {
            if (peek().type != "EOF") {
                throw std::runtime_error("Missing semicolon at the end of statement.");
            }
            break;
        }

        if (!result) {
            result = stmt;
        }
        else {
            result = new ASTNode("SEQUENCE", "", result, stmt);
        }
    }

    if (!result) {
        return new ASTNode("EMPTY", "");
    }
    return result;
}

ASTNode* Parser::parse_statement() {
    if (match("PRINT")) {
        ASTNode* expr = parse_expression();
        return new ASTNode("PRINT", "", expr, nullptr);
    }

    if (peek().type == "VARIABLE") {
        Token varToken = advance();

        if (match("EQUALS")) {
            ASTNode* expr = parse_expression();
            return new ASTNode("ASSIGN", varToken.value, expr, nullptr);
        }
        else {
            current_position--;
        }
    }

    return parse_expression();
}

ASTNode* Parser::parse_expression() {
    ASTNode* node = parse_term();
    while (peek().type == "PLUS" || peek().type == "MINUS") {
        Token op = advance();
        ASTNode* right = parse_term();
        node = new ASTNode(op.type, op.value, node, right);
    }
    return node;
}

ASTNode* Parser::parse_term() {
    ASTNode* node = parse_factor();
    while (peek().type == "STAR" || peek().type == "SLASH") {
        Token op = advance();
        ASTNode* right = parse_factor();
        node = new ASTNode(op.type, op.value, node, right);
    }
    return node;
}

ASTNode* Parser::parse_factor() {
    if (match("MINUS")) {
        ASTNode* right = parse_factor();
        return new ASTNode("UNARY_MINUS", "-", right, nullptr);
    }

    if (match("PLUS")) {
        ASTNode* right = parse_factor();
        return new ASTNode("UNARY_PLUS", "+", right, nullptr);
    }

    if (match("NUMBER")) {
        return new ASTNode("NUMBER", tokens[current_position - 1].value);
    }

    if (match("VARIABLE")) {
        return new ASTNode("VARIABLE", tokens[current_position - 1].value);
    }

    if (match("LPAREN")) {
        ASTNode* node = parse_expression();
        if (!match("RPAREN")) {
            throw std::runtime_error("Missing closing paren");
        }
        return node;
    }

    throw std::runtime_error("Unexpected token: " + peek().value);
}

Token Parser::advance() {
    if (current_position < tokens.size()) {
        return tokens[current_position++];
    }
    return Token{ "EOF", "" };
}

Token Parser::peek() {
    if (current_position < tokens.size()) {
        return tokens[current_position];
    }
    return Token{ "EOF", "" };
}

bool Parser::match(const std::string& type) {
    if (peek().type == type) {
        advance();
        return true;
    }
    return false;
}

void Parser::printNode(ASTNode* node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; i++) std::cout << "  ";

    std::cout << node->type << " (" << node->value << ")\n";
    if (node->left) {
        std::cout << std::string(depth * 2, ' ') << "Left:\n";
        printNode(node->left, depth + 1);
    }
    if (node->right) {
        std::cout << std::string(depth * 2, ' ') << "Right:\n";
        printNode(node->right, depth + 1);
    }
}
