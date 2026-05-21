#pragma once
#include "ast_maker.h"
#include <map>

class Evaluate {
private:
  //Node to grab the tree
  Node input;
  //Set up the first blank frame
  map<string, string> frame;
  //Create the memory array which allows the "stacked" fucntions
  vector<map<string, string>> memory;
  //Create the map that holds the node to execute functions on calls
  map<string, Node> function_memory;
  int node_count;
public:
  Evaluate(Node tree);
  void evaluate(); 
  string evaluate_children(Node node, string context = "");
};
