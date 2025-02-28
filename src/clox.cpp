#include "clox.hpp"
#include "scanner.hpp"
#include "util.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <print>
#include <string>
#include <variant>

int Clox::run_file(std::string_view file_path)
{
  std::ifstream istrm((std::string(file_path)));
  if (!istrm.is_open()) {
    std::println(stderr, "Could not open file: {}", file_path);
    return 1;
  } else {
    std::string input((std::istreambuf_iterator<char>(istrm)), std::istreambuf_iterator<char>());
    Scanner scanner{ input };
    auto [err, tokens] = scanner.scan();
    print_tokens(tokens);
  }
  return 0;
}

int Clox::run_prompt()
{
  std::string input{};
  while (true) {
    std::print("> ");
    std::getline(std::cin, input);
    if (input.empty()) { break; }
    Scanner scanner{ input };
    auto [err, tokens] = scanner.scan();
    print_tokens(tokens);
  }
  return 0;
}

void Clox::report(int line, std::string_view message) { std::println(stderr, "Error on line {}: {}", line, message); }

void Clox::print_tokens(std::vector<Token> const &tokens)
{
  for (const auto &token : tokens) {
    std::print("{} | {} ", token.line, token.lexeme);
    std::visit(util::VariantVisitor(), token.literal);
    std::println();
  }
}
