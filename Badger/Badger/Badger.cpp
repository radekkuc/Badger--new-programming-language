#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::string read_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open : " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    try {
        std::string code = read_file("mycode.txt");

        Lexer lexer(code);
        auto tokens = lexer.tokenize();
        for (auto& t : tokens) {
            std::cout << t.type << " => " << t.value << std::endl;
        }
        Parser parser(tokens);
        ASTNode* root = parser.parse();
        parser.printNode(root);

        Interpreter interpreter;
        interpreter.evaluate(root);

        delete root;
    }
    catch (const std::exception& e) {
        std::cerr << "Błąd: " << e.what() << std::endl;
    }

    return 0;
}