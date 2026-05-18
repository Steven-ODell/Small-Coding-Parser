# slang

A custom interpreted programming language built from scratch in C++. No libraries, no frameworks — a hand-written lexer, recursive descent parser, and AST evaluator.

## Overview

slang is a self-directed systems programming project. The goal is a fully working interpreter pipeline built from first principles, used as a foundation for learning compiler design, LLVM, and low-level systems programming.

## Pipeline

```
source file → lexer → token stream → parser → AST → evaluator → output
```

**Lexer** — reads raw source text character by character and produces a flat stream of typed tokens. Handles keywords, identifiers, string literals, operators, punctuation, and whitespace stripping.

**Parser** — recursive descent parser that consumes the token stream and builds an Abstract Syntax Tree. Implements full operator precedence via a three-level function chain (`check_expression` → `check_term` → `check_factor`).

**Evaluator** — walks the AST and executes the program. Variable storage, scoped function execution, conditional branching.

## Language Features

```js
// variable declaration
let x = 5;
let name = "Steven";

// comparisons
let passing = x > 3;

// conditionals
if (passing) {
    let bonus = (x + 2) * (2 / 1);
    print(bonus);
} else {
    print(x);
}

// functions
fn add(a, b) {
    let result = a + b;
    return result;
}

let total = add(x, 10);
print(total);
```

**Supported:**
- Variable declarations (`let`)
- Arithmetic with correct precedence (`+` `-` `*` `/`)
- Parenthesized grouping
- Comparison operators (`>` `<` `>=` `<=` `==`)
- Conditional branching (`if` / `else`)
- Function declarations with parameters (`fn`)
- Function calls with arguments
- Return values
- String literals
- Built-in `print`

## AST Example

For `let total = add(x, 10);` the parser produces:

```
Declaration | total
   FunctionCall | add
      Identifier | x
      Number | 10
```

For `(x + 2) * (2 / 1)` operator precedence is enforced structurally:

```
Operation | *
   Operation | +
      Identifier | x
      Number | 2
   Operation | /
      Number | 2
      Number | 1
```

Multiplication sits higher in the tree because `check_term` (which handles `*` and `/`) runs deeper in the call stack than `check_expression` (which handles `+` and `-`). The tree shape enforces evaluation order — no runtime precedence logic needed.

## Project Structure

```
├── lexer.h / lexer.cpp       — tokenizer
├── ast_maker.h / ast_maker.cpp — recursive descent parser and AST builder
├── load_file.h               — file loader
├── main.cpp                  — entry point
└── inputfile.slang           — example source file
```

## Build

```bash
g++ -o slang main.cpp lexer.cpp ast_maker.cpp
./slang
```

## Status

- [x] Lexer
- [x] Recursive descent parser
- [x] Operator precedence
- [x] Variable declarations
- [x] If / else blocks
- [x] Function declarations and calls
- [x] Return statements
- [ ] Evaluator
- [ ] Scope stack
- [ ] Standard library (print, etc.)

## Motivation

Built as a stepping stone toward compiler design, LLVM, and systems programming in C++. Every component is written by hand to build genuine understanding of how languages work at the implementation level.
