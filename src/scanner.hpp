#pragma once
#include "token.hpp"
#include "token_type.hpp"
#include <cstddef>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

class Scanner
{
public:
  explicit Scanner(std::string_view source);
  [[nodiscard]] std::vector<Token> const &get_tokens() const { return tokens; }
  [[nodiscard]] std::pair<bool, std::vector<Token>> scan();

private:
  void scan_token();
  [[nodiscard]] bool end_reached() const;
  [[nodiscard]] char peek() const;
  [[nodiscard]] char peek_next() const;
  void scan_string();
  void scan_number();
  void scan_identifier();
  char advance();
  void add_token(TokenType type);
  void add_token(TokenType type, LiteralType literal);
  bool match(char key);

  void report(int line, std::string_view message);

  std::size_t start = 0;
  std::size_t current = 0;
  int line = 1;

  std::string source;
  std::vector<Token> tokens;
  bool error = false;
};
