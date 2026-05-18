#include "lexer.h"
#include <cctype>
#include <string>
#include <iostream>

Lexer::Lexer(string source) {
  input = source;
  current = 0;
  line_counter = 1;
}

vector<Token> Lexer::tokenize() {
  for (size_t char_count = 0; char_count < input.length();) {
    Token t; //Create new token for each "token" lol

    if (isalpha(input[char_count])) {
      string word;
      while (isalpha(input[char_count])) {
        word += input[char_count];
        char_count++;
      }
      if (word == "let") {
        t.type = "Declaration";
        t.value = word;
      }
      else if (word == "if") {
        t.type = "If_Check";
        t.value = grab_condition(char_count);
      }
      else if (word == "else") {
        t.type = "Else_Check";
      }
      else if (word == "fn") {
        t.type = "FunctionAssignment";
      }
      else if (word == "return") {
        t.type = "Return";
      }
      else if (word == "else") {
        t.type = "Else_Check";
      }
      else{
        t.type = "Identifier";
        t.value = word;
      }
      tokens.push_back(t);//Push the token back into the Array(tokens)
      word = "";
    }

    else if (isdigit(input[char_count])) {
      string numbertoken;
      while (isdigit(input[char_count])) {
        numbertoken += input[char_count];
        char_count++;
      }
      t.type = "Number";
      t.value = numbertoken;
      tokens.push_back(t);
      numbertoken = "";
    }

    else if (char_count < input.length() && isspace(input[char_count])) {
      while (isspace(input[char_count])) {
        if (input[char_count] == '\n') {
          t.type = "NewLine";
          t.value = "\\n";
          line_counter++;
        }
        else {
          t.type = "Space";
          t.value = " ";
        }
      tokens.push_back(t);
      char_count++;
      }
    }

    else if (input[char_count] == '=' || input[char_count] == '<' || input[char_count] == '>') {
      if (input[char_count+1] == '=') {
        t.type = "Comparison";
        t.value = input[char_count] + input[char_count+1];
      }
      else if (input[char_count+1] != '=' && input[char_count] == '=') {
        t.type = "Equals";
        t.value = "=";
      }
      else {
        t.type = "Comparison";
        t.value = input[char_count];
      }
      tokens.push_back(t);
      char_count++;
    }
    
    else if (input[char_count] == '+' || input[char_count] == '-' || input[char_count] == '*' || input[char_count] == '/') {
      t.type = "Operation";
      t.value = input[char_count];
      tokens.push_back(t);
      char_count++;
    }

    else if (input[char_count] == '"') {
      char_count++;
      string Literal;
      while (input[char_count] != '"') {
        Literal += input[char_count];
        char_count++;
      }
      t.type = "StringLiteral";
      t.value = Literal;
      tokens.push_back(t);
      char_count++;
    }

    else if (input[char_count] == '\'') {
      char_count++;
      string Literal;
      while (input[char_count] != '\'') {
        Literal += input[char_count];
        char_count++;
      }
      t.type = "StringLiteral";
      t.value = Literal;
      tokens.push_back(t);
      char_count++;
    }

    else if (input[char_count] == '(') {
      t.type = "LParen";
      t.value = "(";
      tokens.push_back(t);
      char_count++;
    }

    else if (input[char_count] == ')') {
      t.type = "RParen";
      t.value = ")";
      tokens.push_back(t);
      char_count++;
    }

    else if (input[char_count] == '{') {
      t.type = "LCBracket";
      t.value = "{";
      tokens.push_back(t);
      char_count++;
    }

    else if (input[char_count] == '}') {
      t.type = "RCBracket";
      t.value = "}";
      tokens.push_back(t);
      char_count++;
    }

    else if (input[char_count] == '[') {
      t.type = "LBracket";
      t.value = "[";
      tokens.push_back(t);
      char_count++;
    }

    else if (input[char_count] == ']') {
      t.type = "RBracket";
      t.value = "]";
      tokens.push_back(t);
      char_count++;
    }

    else if (input[char_count] == ',') {
      t.type = "Comma";
      t.value = ",";
      tokens.push_back(t);
      char_count++;
    }

    else if (input[char_count] == ';') {
      t.type = "EOL";
      t.value = ";";
      tokens.push_back(t);
      char_count++;
    }

    else if (input[char_count] == '!') {
      t.type = "NOT";
      t.value = "!";
      tokens.push_back(t);
      char_count++;
    }

    else if (char_count < input.length()) {
      char_count++;
    }
  }
  return tokens;
}

void Lexer::clean_white_spacesNL() {
  vector<Token> cleaned_tokens;
  for (int token_count = 0; token_count < tokens.size(); token_count++) {
    if (tokens[token_count].type != "NewLine" && tokens[token_count].type != "Space") {
      cleaned_tokens.push_back(tokens[token_count]);
    }
  }
  tokens = cleaned_tokens;
}

void Lexer::clean_white_spaces() {
  vector<Token> cleaned_tokens;
  for (int token_count = 0; token_count < tokens.size(); token_count++) {
    if (tokens[token_count].type != "Space") {
      cleaned_tokens.push_back(tokens[token_count]);
    }
  }
  tokens = cleaned_tokens;
}

void Lexer::print_tokens() {
  for (int token_count = 0; token_count < tokens.size(); token_count++) {
      cout << tokens[token_count].type << ": " << tokens[token_count].value << endl;
    }
}

void Lexer::count_tokens() {
  cout << tokens.size() << ": Tokens\n" << "found in " << line_counter << " lines" << endl;
}

string Lexer::grab_condition(int char_count) {
  char_count++;
  string condition;  
  if (char_count < input.length() && input[char_count] == '(') {
    char_count++;
  }
  while (char_count < input.length() && input[char_count] != ')') {
    condition += input[char_count];
    char_count++;
  }
  return condition;
}

const vector<Token>& Lexer::grab_tokens() const {
  return tokens;
}
