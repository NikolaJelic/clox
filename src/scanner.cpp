#include "scanner.hpp"


#include "clox.hpp"
#include "parsed_tokens.hpp"
#include "token_type.hpp"
#include "util.hpp"
#include <cctype>
#include <charconv>
#include <cmath>
#include <optional>
#include <print>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

namespace {
std::unordered_map<std::string_view, TokenType> const keywords{ { "and", TokenType::AND },
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
}

Scanner::Scanner(std::string_view source) : source(source) {}

bool Scanner::end_reached() const { return current >= source.length(); }

char Scanner::advance()
{
  if (end_reached()) { return '\0'; }
  return source.at(current++);
}

void Scanner::add_token(TokenType type) { add_token(type, std::nullopt); }

void Scanner::add_token(TokenType type, std::optional<LiteralType> literal)
{
  std::string lexeme{ source.substr(start, current - start) };
  tokens.push_back(Token{ .line = line, .type = type, .lexeme = lexeme, .literal = std::move(literal) });
}

bool Scanner::match(char key)
{
  if (end_reached()) { return false; }

  if (source.at(current) != key) { return false; }

  ++current;
  return true;
}

char Scanner::peek() const
{
  if (end_reached()) { return '\0'; }
  return source.at(current);
}

char Scanner::peek_next() const
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
    report(line, "Unterminated string");
    return;
  }

  advance();

  std::string_view value = source.substr(start + 1, current - start - 2);
  add_token(TokenType::STRING, std::string{ value });
}

void Scanner::scan_number()
{
  while (util::is_alphanumeric(peek()) || peek() == '.') { advance(); }
  double val = NAN;
  std::string_view str_val = source.substr(start, current - start);
  auto [ptr, ec] = std::from_chars(str_val.data(), str_val.data() + str_val.size(), val);
  if (ec == std::errc() && ptr == str_val.data() + str_val.size()) {
    add_token(TokenType::NUMBER, val);
  } else {
    report(line, "Invalid number: " + std::string{ str_val });
  }
}

void Scanner::scan_identifier()
{
  while (util::is_alphanumeric(peek())) { advance(); }
  std::string_view lexeme = source.substr(start, current - start);
  auto it = ::keywords.find(lexeme);
  if (it != ::keywords.end()) {
    add_token(it->second);
  } else {
    add_token(TokenType::IDENTIFIER);
  }
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
      report(line, "Unexpected character: " + std::string{ c });
    }
  }
}

ParsedTokens Scanner::scan()
{
  while (!end_reached()) {
    start = current;
    scan_token();
  }

  tokens.push_back(Token{ .line = line, .type = TokenType::END_OF_FILE, .lexeme = "" });
  return { .error = error, .tokens = std::move(tokens) };
}

void Scanner::report(int line, std::string_view message)
{
  error = true;
  Clox::report(line, message);
}
