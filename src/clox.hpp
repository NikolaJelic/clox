#pragma once
#include <string_view>
class Clox
{
public:
  static inline bool error = false;
  static int run_file(std::string_view file_path);
  static int run_prompt();
  static int run(std::string_view line);
  static void report(int line, std::string_view message);
};
