#include "parser.h"
#include "interpreter.h"
#include <iostream>
#include <stdexcept>

int Interpreter::evaluate(ASTNode* node) {
	if (node == nullptr) {
		throw std::runtime_error("Unexpected empty node ");
	}

	if (node->type == "PRINT") {
		int value = evaluate(node->left);
		std::cout << "Printing value: " << value << std::endl;
		return value;
	}

	if (node->type == "SEQUENCE") {
		evaluate(node->left);
		return evaluate(node->right);
	}

	if (node->type == "NUMBER") {
		return std::stoi(node->value);
	}

	if (node->type == "STRING") {
		throw std::runtime_error("Not supported in this version, string in numerical expression");
	}

	if (node->type == "ASSIGN") {
		std::string var_name = node->value;
		int value = evaluate(node->left);
		Value myValue = Value(value);
		assign(var_name, myValue);
		return value;
	}

	if (node->type == "VARIABLE") {
		if (variables.find(node->value) != variables.end()) {
			if (std::holds_alternative<int>(variables[node->value].value)) {
				return std::get<int>(variables[node->value].value);
			}
			throw std::runtime_error("Not supported, string in numerical expression");
		}
		else {
			throw std::runtime_error("Unidentified variable " + node->value);
		}
	}

	if (node->type == "PLUS") {
		return evaluate(node->left) + evaluate(node->right);
	}
	else if (node->type == "MINUS") {
		return evaluate(node->left) - evaluate(node->right);
	}
	else if (node->type == "STAR") {
		return evaluate(node->left) * evaluate(node->right);
	}
	else if (node->type == "SLASH") {
		int right = evaluate(node->right);
		if (right == 0) {
			throw std::runtime_error("Cant divide by 0");
		}
		return evaluate(node->left) / right;
	}

	if (node->type == "UNARY_MINUS") {
		return -evaluate(node->left);
	}
	throw std::runtime_error("Unknown type of node " + node->type);

}

void Interpreter::assign(std::string name, Value& value) {
	variables[name] = value;
}