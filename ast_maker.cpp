#include "ast_maker.h"
#include <vector>
#include <iostream>


TreeBuilder::TreeBuilder(const vector<Token>& t) : tokens(t) {
  current = 0;
}

Node TreeBuilder::create_tree() {
  Node Root;
  Root.type = "Program";
  Root.value = "Main";
  Root.children = parse_input(tokens);
  return Root;
}

vector<Node> TreeBuilder::parse_input(vector<Token> t) {
  vector<Node> tree;
  while (current < t.size()) {
    if (t[current].type == "Declaration" && t[current+1].type == "Identifier") {
      current+=2;
      tree.push_back(check_declaration(t));
    }
    else if (t[current].type == "Identifier" && t[current+1].type == "LParen") {
      tree.push_back(check_function_exec(t));
      current++;
    }
    else if ((t[current].type == "Number" ||t[current].type == "Identifier") && t[current+1].type == "Operation") {
      current++;
      tree.push_back(check_expression(t));
    }
    else if ((t[current].type == "Number" ||t[current].type == "Identifier") && t[current+1].type == "Comparison") {
      current++;
      tree.push_back(check_comparison(t));
    }
    else if (t[current].type == "FunctionAssignment") {
      current++;
      tree.push_back(check_function(t));
    }
    else if (t[current].type == "If_Check") {
      current++;
      tree.push_back(check_if(t));
    }
    else if (t[current].type == "Return") {
      current++;
      tree.push_back(check_return(t));
    }
    else {
      current++;
    }
  }
  return tree;
}

Node TreeBuilder::check_function_exec(vector<Token> t) {
  Node func_exec;
  func_exec.type = "FunctionCall";
  func_exec.value = t[current].value;
  current+=2;

  while (current < t.size() && t[current].type != "RParen") {
    if (t[current].type == "Comma") current++;

    else {
      func_exec.children.push_back(check_expression(t));
    }
  }
  current++;

  return func_exec;
}

Node TreeBuilder::check_return(vector<Token> t) {
  Node return_node;
  return_node.type = t[current].type;
  current++;

  return_node.children.push_back(check_expression(t));

  return return_node;
}

Node TreeBuilder::check_function(vector<Token> t) {
  Node func_node;
  func_node.type = "FunctionDeclaration";
  func_node.value = t[current].value;
  if (t[current].type == "Identifier" && t[current+1].type == "LParen") {
    current+=2;
    Node func_inputs;
    Node func_calls;
    func_inputs.type = "FunctionInputSets";
    func_inputs.value = grab_function_calls(t);
    string identifier;
    for (int i = 0; i < func_inputs.value.length(); i++) {
      if (func_inputs.value[i] != ',') {
        identifier += func_inputs.value[i];
      }
      else {
        func_calls.type = "FunctionInput";
        func_calls.value = identifier;
        func_inputs.children.push_back(func_calls);
        identifier = "";
      }
    }
    if (!identifier.empty()) {
      func_calls.type = "FunctionInput";
      func_calls.value = identifier;
      func_inputs.children.push_back(func_calls);
    }
    func_node.children.push_back(func_inputs);
  }

  Node func_block = collect_block(t);

  func_block.type = "FunctionBlock";
  func_node.children.push_back(func_block);
  return func_node;
}

Node TreeBuilder::check_declaration(vector<Token> t) {
  Node declare_node;
  if (current < t.size() && t[current].type != "Equals") {
    cout << "Expected '=' but got '" << t[current].value << "'\nTrying to Declare '" << t[current-1].value << "'" << endl;
    current++;
    return declare_node;
  }
  declare_node.value = t[current - 1].value;
  current++;
  declare_node.type = "Declaration";

  declare_node.children.push_back(check_comparison(t));
  if (current < tokens.size() && t[current].type  == "EOL") {
    current++;
  }
  return declare_node;
}

Node TreeBuilder::check_comparison(vector<Token> t) {
  Node Left = check_expression(t);
  while (current < t.size() && t[current].type != "EOL") {
    if (t[current].value == ">" || t[current].value == "<" || t[current].value == ">=" || t[current].value == "<=") {
      Node op_node;
      op_node.type = "Comparison";
      op_node.value = t[current].value;
      current++;

      Node Right = check_expression(t);

      op_node.children.push_back(Left);
      op_node.children.push_back(Right);
      Left = op_node;
    }
    else {
      break;
    }
  }
  return Left;
}

Node TreeBuilder::check_if(vector<Token> t) {
  Node condition_node;
  if (current < t.size() && t[current].type == "LParen") {
    current++;
    condition_node.type = "Condition";
    condition_node.children.push_back(check_expression(t));
  }
  current++;
  Node if_node = collect_block(t);
  if_node.type = "If_Block"; 

condition_node.children.push_back(if_node);

if (current < t.size() && t[current].type == "Else_Check") {
  current++;
  Node else_node = collect_block(t);
  else_node.type = "Else_Block";
  condition_node.children.push_back(else_node);
}

return condition_node;
}

//Check the lowest "priority" first. This checks the throw the left node to be checked instantly
//
Node TreeBuilder::check_expression(vector<Token> t) {
  Node Left = check_term(t);
  while (current < t.size() && t[current].type != "EOL") {
    if (t[current].value == "+" || t[current].value == "-") {
      Node op_node;
      op_node.type = "Operation";
      op_node.value = t[current].value;
      current++;

      //Creat the second node after establishing the "base node" for the Operation
      //now check if this has any hgiher "priority" items
      Node Right = check_term(t);

      op_node.children.push_back(Left);
      op_node.children.push_back(Right);
      Left = op_node;
    }
    else {
      break;
    }
  }
  return Left;
}

//Check the next "priority" being the * and /.
//With this we do the same going down to "factor"
//aka check what is in the "()"
Node TreeBuilder::check_term(vector<Token> t) {
  Node Left = check_factor(t);
  while (current < t.size() && t[current].type != "EOL") {
    if (t[current].value == "*" || t[current].value == "/") {
      Node op_node;
      op_node.type = "Operation";
      op_node.value = t[current].value;
      current++;
      
      Node Right = check_factor(t);
      op_node.children.push_back(Left);
      op_node.children.push_back(Right);
      Left = op_node;
    }
    else {
      break;
    }
  }
  return Left;
}

//At this point you are inside () just check what is inside there
Node TreeBuilder::check_factor(vector<Token> t) {
  Node node;

  //If you find another function go deeper
  if (t[current].type == "Identifier" && t[current+1].type == "LParen") {
    return check_function_exec(t);
  }
  
  else if (t[current].type == "Number" || t[current].type == "Identifier" || t[current].type == "StringLiteral") {
    node.type = t[current].type;
    node.value = t[current].value;
    current++;
    return node;
  }

  else if (t[current].type == "Return") {
    node.type = t[current].type;
    node.children.push_back(check_return(t));
    current++;
  }

  //Find more "()" then go in and check that aswell
  if (t[current].type == "LParen") {
    current++;
    node = check_expression(t);
    current++;
    return node;
  }
  return node;
}

void TreeBuilder::print_tree(Node tree, int depth) {
  for (int d = depth; d > 0; d--) { 
    cout << "   ";
  }
  cout << tree.type << " | " << tree.value << endl;
  
  for (int i = 0; i < tree.children.size(); i++) {
    print_tree(tree.children[i], depth+1);
  }
}

//Function to just grab what is in the "()" to then use it later in the tree walking
string TreeBuilder::grab_function_calls(vector<Token> t) {
  string calls;
  if (tokens[current].type == "LParen") current++;
  
  while (current < tokens.size() && tokens[current].type != "RParen") {
    calls+= tokens[current].value;
    current++;
  }

  if (tokens[current].type == "RParen") current++;
  
  return calls; 
}

//Function created to simplify grabbing what is inside a "{}" set 
Node TreeBuilder::collect_block(vector<Token> t) {
  Node node;
  if (t[current].type == "LCBracket") current++; 
  while (current < t.size() && t[current].type != "RCBracket") {
    if (t[current].type == "EOL") current++; 
      
    else if (t[current].type == "Declaration" && t[current+1].type == "Identifier") {
      current+=2;
      node.children.push_back(check_declaration(t));
    }

    else if (t[current].type == "Identifier" && t[current+1].type == "LParen") {
      node.children.push_back(check_function_exec(t));
      current++;
    }
    
    else if (t[current].type == "Return") {
      node.type = t[current].type;
      node.children.push_back(check_return(t));
      current++;
    }

    else current++;
  }
  if (t[current].type == "RCBracket") current++;
  return node;
}
