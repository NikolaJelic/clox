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
  if (auto istrm = std::ifstream{ (std::string(file_path)) }) {
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
  const auto print_visitor = util::overloads{ [](auto &val) { std::println("{}", val); } };

  for (const auto &token : tokens) {
    std::print("{} | {}  ", token.line, token.lexeme);
    if (token.literal.has_value()) { std::visit(print_visitor, token.literal.value()); }
    std::println();
  }
}
