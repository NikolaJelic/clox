#pragma once
#include "token.hpp"
#include "token_type.hpp"
#include <cstddef>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

class Scanner
{
public:
  explicit Scanner(std::string_view source);
  [[nodiscard]] std::vector<Token> const &get_tokens() const { return tokens; }

private:
  void scan();
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
  std::size_t start = 0;
  std::size_t current = 0;
  int line = 1;

  std::string source;
  std::vector<Token> tokens;

  std::unordered_map<std::string_view, TokenType> keywords{ { "and", TokenType::AND },
    { "class", TokenType::CLASS },
    { "else", TokenType::ELSE },
    { "false", TokenType::FALSE },
    { "for", TokenType::FOR },
    { "fun", TokenType::FUN },
    { "if", TokenType::IF },
    { "nil", TokenType::NIL },
    { "or", TokenType::OR },
    { "print", TokenType::PRINT },
    { "return", TokenType::RETURN },
    { "super", TokenType::SUPER },
    { "this", TokenType::THIS },
    { "true", TokenType::TRUE },
    { "var", TokenType::VAR },
    { "while", TokenType::WHILE } };
};
