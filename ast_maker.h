#pragma once
#include "lexer.h"
#include <vector>

struct Node {
  string type;
  string value;
  vector<Node> children;
};

class TreeBuilder {
private:
  int current;
  vector<Token> tokens;
public:
  TreeBuilder(const vector<Token>& tokens);
  Node create_tree();
  vector<Node> parse_input(vector<Token> t);
  Node check_return(vector<Token> t);
  Node check_function_exec(vector<Token> t);
  Node check_declaration(vector<Token> t);
  Node check_function(vector<Token> t);
  Node check_expression(vector<Token> t);
  Node check_if(vector<Token> t);
  Node check_comparison(vector<Token> t);
  Node check_term(vector<Token> t);
  Node check_factor(vector<Token> t);
  void print_tree(Node tree, int depth);
  string grab_function_calls(vector<Token> t);
  Node collect_block(vector<Token> t);
};
