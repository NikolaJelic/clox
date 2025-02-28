#pragma once

#include <print>
#include <variant>
namespace util {

constexpr bool is_alpha(char c) noexcept { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

constexpr bool is_digit(char c) noexcept { return c >= '0' && c <= '9'; }

constexpr bool is_alphanumeric(char c) noexcept { return is_alpha(c) || is_digit(c); }

struct VariantVisitor
{
  void operator()(const std::monostate & /*unused*/) { std::print("_"); }

  template<typename T> void operator()(const T &value) { std::print("{}", value); }
};

}// namespace util
