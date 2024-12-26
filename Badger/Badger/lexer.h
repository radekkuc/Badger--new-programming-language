#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

struct Token {
	std::string type;
	std::string value;
};

class Lexer {
public:
	Lexer(const std::string& input);
	std::vector<Token> tokenize();

private:
	std::string input;
	size_t current_position;
	char advance();
	char peek();
	void skip_white_space();
	Token read_number();
	Token read_identifier();
};


#endif