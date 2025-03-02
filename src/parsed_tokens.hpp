#pragma once

#include "token.hpp"
#include <vector>
struct ParsedTokens
{
  bool error{};
  std::vector<Token> tokens;
};
