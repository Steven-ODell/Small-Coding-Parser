#pragma once
#include <string>
#include <vector>

using namespace std;

struct Token {
  string type;
  string value;
};

class Lexer {
private:
  string input;
  int current;
  int line_counter;
  vector<Token> tokens;
public:
  Lexer(string source);
  vector<Token> tokenize();//turn source into a Token push to tokens vector
  void clean_white_spacesNL();//clean but strip spaces and newlines
  void clean_white_spaces();//clean but leave new lines
  void print_tokens();
  void count_tokens();
  string grab_condition(int char_count);
  const vector<Token>& grab_tokens() const;
};
