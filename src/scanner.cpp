#include "scanner.hpp"


#include "clox.hpp"
#include "token_type.hpp"
#include "util.hpp"
#include <cctype>
#include <charconv>
#include <cmath>
#include <print>
#include <utility>

Scanner::Scanner(std::string_view source) : source(source)
{
  std::println("Starting scanner on {}", source);
  scan();
}

[[nodiscard]] bool Scanner::end_reached() const { return current >= source.length(); }

char Scanner::advance()
{
  if (end_reached()) { return '\0'; }
  return source.at(current++);
}

void Scanner::add_token(TokenType type) { add_token(type, {}); }

void Scanner::add_token(TokenType type, LiteralType literal)
{
  auto lexeme = source.substr(start, current - start);
  std::println("{}", lexeme);
  tokens.push_back({ line, type, lexeme, std::move(literal) });
}

bool Scanner::match(char key)
{
  if (end_reached()) { return false; }

  if (source.at(current) != key) { return false; }

  ++current;
  return true;
}

[[nodiscard]] char Scanner::peek() const
{
  if (end_reached()) { return '\0'; }
  return source.at(current);
}

[[nodiscard]] char Scanner::peek_next() const
{
  if (current + 1 >= source.length()) { return '\0'; }
  return source.at(current + 1);
}


void Scanner::scan_string()
{
  while (peek() != '"' && !end_reached()) {
    if (peek() == '\n') { ++line; }
    advance();
  }

  if (end_reached()) {
    Clox::report(line, "Unterminated string");
    return;
  }

  advance();

  std::string value = source.substr(start + 1, current - start - 1);
  add_token(TokenType::STRING, value);
}

void Scanner::scan_number()
{
  while (util::is_digit(peek())) { advance(); }
  if (peek() == '.' && (util::is_digit(peek_next()))) {
    advance();
    while (util::is_digit(peek())) { advance(); }
  }

  float val = NAN;
  std::string str_val = source.substr(start, current - start);
  auto [ptr, ec] = std::from_chars(str_val.data(), str_val.data() + str_val.size(), val);
  if (ec == std::errc()) { add_token(TokenType::NUMBER, val); }
}

void Scanner::scan_identifier()
{
  while (util::is_alphanumeric(peek())) { advance(); }
  std::string lexeme = source.substr(start, current - start);
  auto it = keywords.find(lexeme);
  TokenType type = TokenType::IDENTIFIER;
  if (it != keywords.end()) { type = it->second; }

  add_token(type);
}

void Scanner::scan_token()
{
  char c = advance();
  switch (c) {
  case '(':
    add_token(TokenType::LEFT_PAREN);
    break;
  case ')':
    add_token(TokenType::RIGHT_PAREN);
    break;
  case '{':
    add_token(TokenType::LEFT_BRACE);
    break;
  case '}':
    add_token(TokenType::RIGHT_BRACE);
    break;
  case ',':
    add_token(TokenType::COMMA);
    break;
  case '.':
    add_token(TokenType::DOT);
    break;
  case '-':
    add_token(TokenType::MINUS);
    break;
  case '+':
    add_token(TokenType::PLUS);
    break;
  case ';':
    add_token(TokenType::SEMICOLON);
    break;
  case '*':
    add_token(TokenType::STAR);
    break;
  case '!':
    add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
    break;
  case '=':
    add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
    break;
  case '<':
    add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
    break;
  case '>':
    add_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
    break;
  case '/':
    if (match('/')) {
      while (peek() != '\n' && !end_reached()) { advance(); }
    } else {
      add_token(TokenType::SLASH);
    }
    break;

  case ' ':
  case '\r':
  case '\t':
    break;

  case '\n':
    line++;
    break;

  case '"':
    scan_string();
    break;
  default:
    if (util::is_digit(c)) {
      scan_number();
    } else if (util::is_alpha(c)) {
      scan_identifier();
    } else {
      Clox::report(line, "Unexpected character.");
    }
  }
}

void Scanner::scan()
{
  while (!end_reached()) {
    start = current;
    scan_token();
  }

  tokens.push_back({ line, TokenType::END_OF_FILE, "", {} });
}
