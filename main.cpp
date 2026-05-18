#include "lexer.h"
#include "ast_maker.h"
#include "load_file.h"
#include "evaluate.h"
#include <iostream>


int main() {
  //Print the thing being processd 
  string loaded_file = load_file();
  cout << "Processing this file:\n" << loaded_file << endl;

  //Load the file into the lexer(now a variable in the class)
  Lexer lexer(loaded_file);

  //Tokenize the input file sting
  lexer.tokenize();

  //Clean white spaces including NewLines
  lexer.clean_white_spacesNL();
  
  //Clean white spaces excluding NewLines
  //lexer.clean_white_spaces();

  //lexer.print_tokens();
  //lexer.count_tokens();

  //Call the TreeBuilder and feed it the read only reference
  //to the array in memory using grab_tokens()
  TreeBuilder tb(lexer.grab_tokens());
  
  //Now that the tb has loaded in the tokens it can begin building
  //an ast tree
  Node tree = tb.create_tree();


  // print the tree that was built
  tb.print_tree(tree, 0);

  Evaluate eval(tree);
  
  eval.evaluate();

  return 0;
}
