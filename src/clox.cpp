#include "clox.hpp"
#include "scanner.hpp"
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
    std::println("Could not open file: {}", file_path);
    return 1;
  } else {
    std::string input((std::istreambuf_iterator<char>(istrm)), std::istreambuf_iterator<char>());
    Scanner scanner{ input };
    print_tokens(scanner.get_tokens());
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
    print_tokens(scanner.get_tokens());
  }
  return 0;
}

void Clox::report(int line, std::string_view message)
{
  Clox::error = true;
  std::println("Error on line {}: {}", line, message);
}

void Clox::print_tokens(std::vector<Token> const &tokens)
{
  for (const auto &token : tokens) {
    std::cout << token.line << " | " << token.lexeme << " | ";

    std::visit(
      [](const auto &value) {
        if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::monostate>) {
          std::print("MONOSTATE\n");
        } else {
          std::print("{}\n", value);
        }
      },
      token.literal);
  }
}
