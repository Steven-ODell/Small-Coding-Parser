#include "evaluate.h"
#include <iostream>
#include <string>


//Set up Evaluate node which holds the tree within the class as input.
//Frame is in the header. This allows me to set up a blank space in memory to start the output.
Evaluate::Evaluate(Node tree) {
  input = tree;
  node_count = 1;
  memory.push_back(frame);
}

//Loop through the tree and walk the nodes. If it has children go in and then walk that.
//This allows the viewing of each deeper nested node in the tree
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

//Evaluate function walks tree and returns outputs based on what the base node is 
//that it is currently on
string Evaluate::evaluate_children(Node node, string context) {
  //Catch a parse type and then set the string to be fed back in to "process"
  //the rest of the child nodes
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
  //When caught we catch the left and right values which are children 0 and 1.
  //Then a simple check and return
  else if (node.type == "Comparison") {
    //node.children[0] is the left side
    string left = evaluate_children(node.children[0]);
    //node.children[1] is the right side
    string right = evaluate_children(node.children[1]);
    cout << "Comparing: '" << node.value << "' " << left << " | " << right << " | node " << node_count << endl;
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
  //
  else if (node.type == "Operation") {
    //node.children[0] is the left side
    string left = evaluate_children(node.children[0]);
    //node.children[1] is the right side
    string right = evaluate_children(node.children[1]);
    cout << "Operating: '" << node.value << "' " << left << " | " << right << " | node " << node_count << endl;
    if (left.empty() || right.empty()) return "";
    int l = stoi(left);
    int r = stoi(right);
    if (node.value == "+") return to_string(l+r);
    if (node.value == "-") return to_string(l-r);
    if (node.value == "*") return to_string(l*r);
    if (node.value == "/") return to_string(l/r);
  }
  
  //TODO
  //add debug to make it so that it shows what the comparitor is
  
  //Catch the Condition node.
  //Evaluate the condition (node.children[0]) and then check the output with
  //"true" it loops through the node and executes the if condition (node.children[1])
  //or if not then it execute the else block executes (node.children[2]) 
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
  
  //Catch and send the function to be stored in function_memory so that later it
  //can be plugged into and called with function_memory[call]
  else if (node.type == "FunctionDeclaration") {
    cout << "Declared Function: " << node.value << " | node " << node_count << endl;
    function_memory[node.value] = node;
  }
  //Catch the function call and set up the new node in the main "stack" that pulls in
  //the functions and sets it up to pull in the new data
  else if (node.type == "FunctionCall") {
    cout << "Called Function: " << node.value << " | node " << node_count << endl;
    //catch the print call and just set up a node to evaluate and print result
    if (node.value  == "print") {
      string print_string;
      for (int i = 0; i < node.children.size(); i++) {
      print_string = evaluate_children(node.children[i]);
      }
      cout << "PRINTING: " << print_string << " | node " << node_count << endl;
      return "";
    }
    
    //Set up node that is reference to the function stored in "memory"
    Node fn  = function_memory[node.value];
    vector<string> args_holder;
    //loop through the array that holds the args
    for (int i = 0; i < node.children.size(); i++) {
      args_holder.push_back(evaluate_children(node.children[i]));
    }
    //set up blank map to do the function in
    memory.push_back(map<string, string>());
    //set the params within that map linked to the args. 
    for (int dec = 0; dec < fn.children[0].children.size(); dec++){
      string param = fn.children[0].children[dec].value;
      string arg = args_holder[dec];
      memory.back()[param] = arg;
    }
    string result  = "";
    //loop through the function block node in children and evaluate with new params 
    for (int ev = 0; ev < fn.children[1].children.size(); ev++) {
      string val = evaluate_children(fn.children[1].children[ev]);
      if (!val.empty()) result = val;
    }
    //function is done and values are already stored
    memory.pop_back();
    return result;
  }
  return "";
}
