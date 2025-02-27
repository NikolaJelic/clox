#pragma once

#include "token_type.hpp"
#include <string>

struct Token
{
  int line;
  TokenType type;
  std::string lexeme;
  LiteralType literal;
};
