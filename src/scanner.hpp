#pragma once
#include <string>

class Scanner
{
public:
  explicit Scanner(std::string_view data);

private:
  std::string data;
};
