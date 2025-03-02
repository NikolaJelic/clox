#pragma once

#include "token_type.hpp"
#include <optional>
#include <string>
#include <variant>
using LiteralType = std::variant<std::string, bool, double>;


struct Token
{
  int line{};
  TokenType type{};
  std::string lexeme;
  std::optional<LiteralType> literal;
};
