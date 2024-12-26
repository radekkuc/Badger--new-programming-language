#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"
#include <unordered_map>
#include <variant>


class Value {
public:
    using ValueType = std::variant<int, std::string>;
    ValueType value;

    Value() {};
    Value(int v) : value(v) {}
    Value(std::string v) : value(v) {}

    std::string to_string() const {
        if (std::holds_alternative<int>(value)) {
            return std::to_string(std::get<int>(value));
        }
        else if (std::holds_alternative<std::string>(value)) {
            return std::get<std::string>(value);
        }
        else {
            return "Unknown";
        }
    }
};

class Interpreter {
public:
    std::unordered_map<std::string, Value> variables;

public:
    int evaluate(ASTNode* node);
    void assign(std::string name, Value& value);
};


#endif 