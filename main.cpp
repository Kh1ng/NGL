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
  TokenType;
  std::optional<std::string> value {};
};

std::vector<char> buf;

inline std::vector<Token> tokenize(const std::string& str) {

  std::vector<Token> tokens = {};

  for (int i = 0; i < str.length(); i++) {
    char c = str.at(i);
    if(std::isalpha(c)) {
      buf.push_back(c);
      i++;
      while (std::isalnum(str.at(i))) {
        buf.push_back(str.at(i));
      }
      i--;
      if (std::string(buf.begin(), buf.end()) == "return") {
        tokens.push_back({ .type = TokenType::_return });
        buf.clear();
      } else {
        std::cerr << "unknown type" << std::endl;
        exit(EXIT_FAILURE);
      }
    }
  }
}

int main(int argc, char* argv[]) {
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

  tokenize(contents);

  return 0;
}
