#include "lexer.h"
#include <stdexcept>
#include <iostream>

Lexer::Lexer(const std::string& input) : input(input), current_position(0) {}

std::vector<Token> Lexer::tokenize() {
	std::vector<Token> tokens;
	while (current_position < input.size()) {
		char current_char = peek();
		if (isspace(current_char)) {
			skip_white_space();
		}
		else if (isdigit(current_char)) {
			tokens.push_back(read_number());
		}
		else if (current_char == '+') {
			tokens.push_back(Token{ "PLUS", "+" });
			advance();
		}
		else if (current_char == '-') {
			tokens.push_back(Token{ "MINUS", "-" });
			advance();
		}
		else if (current_char == '*') {
			tokens.push_back(Token{ "STAR", "*" });
			advance();
		}
		else if (current_char == '/') {
			tokens.push_back(Token{ "SLASH", "/" });
			advance();
		}
		else if (current_char == '(') {
			tokens.push_back(Token{ "LPAREN", "(" });
			advance();
		}
		else if (current_char == ')') {
			tokens.push_back(Token{ "RPAREN", ")" });
			advance();
		}
		else if (input.substr(current_position, 5) == "print") {
			tokens.push_back(Token{ "PRINT", "print" });
			current_position += 5;
		}
		else if (isalpha(current_char)) {
			tokens.push_back(read_identifier());
		}
		else if (current_char == '=') {
			tokens.push_back(Token{ "EQUALS", "=" });
			advance();
		}
		else if (current_char == ';') {
			tokens.push_back(Token{ "SEMICOLON", ";" });
			advance();
		}
		else {
			throw std::runtime_error("Unknown sign " + std::string(1, current_char));
		}
	}
	return tokens;
}

char Lexer::advance() {
	return input[current_position++];
}

void Lexer::skip_white_space() {
	while (current_position < input.size() &&
		(std::isspace(input[current_position]) || input[current_position] == '\r')) {
		current_position++;
	}
}

char Lexer::peek() {
	return input[current_position];
}

Token Lexer::read_number() {
	size_t start = current_position;
	while (current_position < input.size() && isdigit(input[current_position])) {
		current_position++;
	}
	return Token{ "NUMBER", input.substr(start, current_position - start) };
}

Token Lexer::read_identifier() {
	std::string identifier;
	while (current_position < input.size() && (isalnum(peek()) || peek() == '_')) {
		identifier += advance();
	}
	return Token{ "VARIABLE", identifier };
}
