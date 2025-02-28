#include "clox.hpp"
#include <cstdio>
#include <print>

int main(int argc, char *argv[])
{
  if (argc == 2) {
    Clox::run_file(std::string_view(argv[1]));
  } else if (argc == 1) {
    Clox::run_prompt();
  } else {
    std::print(stderr, "Invalid arguments!");
    return 1;
  }
  return 0;
}
