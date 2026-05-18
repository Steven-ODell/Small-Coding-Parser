#include <fstream>
#include <sstream>
#include <string>

inline std::string load_file() {
  std::ifstream file("/home/saoii/cpp_proj/ParserV2/inputfile.slang");
  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string source = buffer.str();
  return source;
}
