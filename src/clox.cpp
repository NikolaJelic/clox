#include "clox.hpp"
#include <fstream>
#include <iostream>
#include <iterator>
#include <print>
#include <string>

int Clox::run_file(std::string_view file_path)
{
  std::ifstream istrm((std::string(file_path)));
  if (!istrm.is_open()) {
    std::println("Could not open file: {}", file_path);
    return 1;
  } else {
    std::string input((std::istreambuf_iterator<char>(istrm)), std::istreambuf_iterator<char>());
    std::println("{}", input);
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

    std::println("{}", input);
  }
  return 0;
}

void Clox::report(int line, std::string_view message) { std::println("Error on line {}: {}", line, message); }
