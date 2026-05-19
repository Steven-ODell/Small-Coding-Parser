#include "evaluate.h"
#include <iostream>
#include <string>


Evaluate::Evaluate(Node tree) {
  input = tree;
  node_count = 1;
  memory.push_back(frame);
}

void Evaluate::evaluate() {
  for (int i = 0; i < input.children.size(); i++){
    evaluate_children(input.children[i]);
    node_count++;
  }
  for (map<string, string> frame : memory) {
    for (pair<string, string> entry : frame) {
      cout << entry.first << " = " << entry.second << endl;
  }
}
}

string Evaluate::evaluate_children(Node node, string context) {
  if (node.type == "Declaration") {
    if (node.children.size() > 0) {
      cout << "Setting identifier " << node.value << " | node " << node_count << endl;
      string declared = evaluate_children(node.children[0], node.value);
      memory.back()[node.value] = declared;
      return declared;
    }
    else {
      return memory.back()[node.value];
    }
  }
  else if (node.type == "Return") {
    string val = evaluate_children(node.children[0]);
    cout << "RETURN: " << val << " | node " << node_count << endl;
    return val;
  }
  else if (node.type == "StringLiteral") {
    cout << "Setting StringLiteral " << node.value << " | node " << node_count << endl;
    return node.value;
  }
  else if (node.type == "Identifier") {
    cout << "Looking up identifier " << node.value << " | node " << node_count << endl;
    return memory.back()[node.value];
  }
  else if (node.type == "Number") {
    cout << "Digit call " << node.value << " | node " << node_count << endl;
    return node.value;
  }
  else if (node.type == "Comparison") {
    //node.children[0] is the left side
    string left = evaluate_children(node.children[0]);
    //node.children[1] is the right side
    string right = evaluate_children(node.children[1]);
    cout << "Comparing " << left << " | " << right << " | node " << node_count << endl;
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
    string condition = evaluate_children(node.children[0]);
    cout << "Checking if condition node " << node_count << endl;
    if (condition == "true") {
      cout << "Executing if condition node " << node_count << endl;
      for (int  i = 0; i < node.children[1].children.size(); i++) {
        evaluate_children(node.children[1].children[i]);
      }
    }
    else {
      cout << "Executing else condition node " << node_count << endl;
      for (int  i = 0; i < node.children[2].children.size(); i++) {
        evaluate_children(node.children[2].children[i]);
      }
    }

  }
  else if (node.type == "FunctionDeclaration") {
    function_memory[node.value] = node;
  }
  else if (node.type == "FunctionCall") {
    cout << "Called Function: " << node.value << " | node " << node_count << endl;
    if (node.value  == "print") {
      string print_string;
      for (int i = 0; i < node.children.size(); i++) {
      print_string = evaluate_children(node.children[i]);
      }
      cout << "PRINTING: " << print_string << endl;
      return "";
    }
    Node fn  = function_memory[node.value];
    vector<string> args_holder;
    for (int i = 0; i < node.children.size(); i++) {
      args_holder.push_back(evaluate_children(node.children[i]));
    }
    memory.push_back(map<string, string>());
    for (int dec = 0; dec < fn.children[0].children.size(); dec++){
      string param = fn.children[0].children[dec].value;
      string arg = args_holder[dec];
      memory.back()[param] = arg;
    }
    string result  = "";
    for (int ev = 0; ev < fn.children[1].children.size(); ev++) {
      string val = evaluate_children(fn.children[1].children[ev]);
      if (!val.empty()) result = val;
    }
    memory.pop_back();
    return result;
  }
  return "";
}
