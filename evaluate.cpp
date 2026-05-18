#include "evaluate.h"
#include <iostream>
#include <string>


Evaluate::Evaluate(Node tree) {
  input = tree;
  node_count = 1;
}

void Evaluate::evaluate() {
  for (int i = 0; i < input.children.size(); i++){
    evaluate_children(input.children[i]);
    node_count++;
  }
  for (pair<string, string> entry : memory) {
    cout << entry.first << " = " << entry.second << endl;
  }
}

string Evaluate::evaluate_children(Node node, string context) {
  if (node.type == "Declaration") {
    if (node.children.size() > 0) {
      cout << "Node: " << node_count << " | '" << node.value << "' setting identifier" << endl;
      string declared = evaluate_children(node.children[0], node.value);
      memory[node.value] = declared;
      return declared;
    }
    else {
      return memory[node.value];
    }
  }
  else if (node.type == "StringLiteral") {
    cout << "node: " << node_count << " | '" << node.value << "' stringliteral" << endl;
    return node.value;
  }
  else if (node.type == "Identifier") {
    cout << "Node: " << node_count << " | '" << node.value << "' looking up identifier" << endl;
    return memory[node.value];
  }
  else if (node.type == "Number") {
    cout << "Node: " << node_count << " | '" << node.value << "' digit" << endl;
    return node.value;
  }
  else if (node.type == "Comparison") {
    //node.children[0] is the left side
    string left = evaluate_children(node.children[0]);
    //node.children[1] is the right side
    string right = evaluate_children(node.children[1]);
    cout << "Node: " << node_count << " | '" << node.value << "' comparing " << left << " | " << right << endl;
    if (left.empty() || right.empty()) return "";
    int l = stoi(left);
    int r = stoi(right);
    if (node.value == ">=") {
      if (l>=r) return "true";
      else return "false";
    }
    else if (node.value == "<=") {
      if (l<=r) return "true";
      else return "false";
    }
    else if (node.value == "<") {
      if (l<r) return "true";
      else return "false";
    }
    else if (node.value == ">") {
      if (l>r) return "true";
      else return "false";
    }
    else return node.value;
  }
  else if (node.type == "Operation") {
    //node.children[0] is the left side
    string left = evaluate_children(node.children[0]);
    //node.children[1] is the right side
    string right = evaluate_children(node.children[1]);
    cout << "Node: " << node_count << " | '" << node.value << "' operating: " << left << " | " << right << endl;
    if (left.empty() || right.empty()) return "";
    int l = stoi(left);
    int r = stoi(right);
    if (node.value == "+") return to_string(l+r);
    if (node.value == "-") return to_string(l-r);
    if (node.value == "*") return to_string(l*r);
    if (node.value == "/") return to_string(l/r);
  }
  else if (node.type == "Condition") {

  }
  return "";
}
