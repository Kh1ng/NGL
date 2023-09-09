#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <optional>

enum class TokenType {
    _return,
    int_lit,
    semicolon
};

struct Token {
    TokenType type;
    std::optional<std::string> value{};
};

std::vector<char> buf;

inline std::vector<Token> tokenize(const std::string &str) {

    std::vector<Token> tokens = {};

    for (int i = 0; i < str.length(); i++) {
        char c = str.at(i);
        if (std::isalpha(c)) {
            buf.push_back(c);
            i++;
            while (std::isalnum(str.at(i))) {
                buf.push_back(str.at(i));
                i++;
            }
            i--;
            if (std::string(buf.begin(), buf.end()) == "return") {
                tokens.push_back({.type = TokenType::_return});
                buf.clear();
                continue;
            } else {
                std::cerr << "unknown type" << std::endl;
                exit(EXIT_FAILURE);
            }
        } else if (std::isdigit(c)) {
            buf.push_back(c);
            i++;
            while (std::isdigit(str.at(i))) {
                buf.push_back(str.at(i));
                i++;
            }
            i--;
            tokens.push_back({.type = TokenType::int_lit, .value = std::string(buf.begin(), buf.end())});
            buf.clear();
        } else if (c == ';') {
            tokens.push_back({.type = TokenType::semicolon});
        } else if (isspace(c)) {
            continue;
        } else {
            std::cerr << "unknown type" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    return tokens;
}

std::string tokens_to_asm(std::vector<Token> tokens) {
    std::stringstream output;
    output << "global _start\n_start:\n";
    if (tokens.at(0).type == TokenType::_return && tokens.at(1).type == TokenType::int_lit &&
        tokens.at(2).type == TokenType::semicolon) {
        output << "     mov rax, 60\n";
        output << "     mov rdi, " << tokens.at(1).value.value() << std::endl;
        output << "     syscall\n";
    } else {
        std::cerr << "bad return" << std::endl;
        exit(EXIT_FAILURE);
    }

    return output.str();
}

int main(int argc, char *argv[]) {
    //requires argument to compile
    if (argc != 2) {
        std::cerr << "Ya done messed up A-a-ron" << std::endl;
        std::cerr << "Correct format: NGL <input.ngl>" << std::endl;
        return EXIT_FAILURE;
    }

    std::stringstream compileMe;
    std::string contents;
    {
        std::fstream input(argv[1], std::ios::in);
        compileMe << input.rdbuf();
        contents = compileMe.str();
    }

    std::cout << contents;

    std::vector<Token> tokens = tokenize(contents);
    std::cout << tokens_to_asm(tokens);

    {
        std::fstream output("out.asm", std::ios::out);
        output << tokens_to_asm(tokens);
    }

    system("nasm -f elf64 out.asm -o out.o");
    system("ld out.o -o out");

    return 0;
}
