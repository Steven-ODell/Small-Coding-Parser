#pragma once
#include "ast_maker.h"
#include <map>

class Evaluate {
private:
  Node input;
  map<string, string> frame;
  vector<map<string, string>> memory;
  map<string, Node> function_memory;
  int node_count;
public:
  Evaluate(Node tree);
  void evaluate(); 
  string evaluate_children(Node node, string context = "");
};
