#pragma once

#include "token_type.hpp"
#include <string>
#include <variant>

struct Token
{
  int line;
  TokenType type;
  std::string lexeme;
  std::variant<std::string, bool, float, int> literal;
};
