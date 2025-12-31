# SimpleLang Compiler (Lexer + Parser)

*A minimal educational compiler written in C for the **SimpleLang** language.*

---

## 📌 Overview

This project implements a **mini compiler** for a simplified programming language called **SimpleLang**, commonly used in **compiler design courses**.

The project has evolved from a **standalone lexer (scanner)** into a **multi-phase compiler**, and is designed to be **clean, readable, and extensible**.

The compiler currently supports:

* **Lexical Analysis (Lexer / Scanner)** ✅
* **Syntax Analysis (Parser)** ✅

And is structured to be extended into:

* Semantic analysis
* AST processing
* Interpretation or code generation

This project is ideal for:

* Compiler design assignments
* Deep learning of lexer & parser interaction
* Building a full compiler pipeline step by step

---

## 🧠 Compiler Architecture

```
Source Code (.sl)
        ↓
      Lexer
        ↓
   Token Stream
        ↓
      Parser
        ↓
   Syntax Validation / AST
```

---

## 🚀 Features

### 🔹 General

* Fully written in **ANSI C / C99**
* Clear phase separation (lexer / parser)
* Line-based error reporting
* Educational, readable implementation

---

### 🔹 Lexer (Scanner)

The lexer reads the source file character-by-character and converts it into a stream of tokens.

#### Supported Tokens

**Keywords**

* `if`
* `else`
* `while`
* `return`

**Identifiers**

```
[A-Za-z_][A-Za-z0-9_]*
```

**Numbers**

```
[0-9]+
```

**String Literals**

```
"([^"\\]|\\.)*"
```

* No newline allowed inside strings
* Supported escapes: `\"` , `\\`

**Operators**

| Category   | Tokens                 |
| ---------- | ---------------------- |
| Arithmetic | `+  -  *  /`           |
| Assignment | `=`                    |
| Comparison | `==  !=  <=  >=  <  >` |

**Separators**

```
( ) { } , ;
```

---

### 🔹 Comment Handling

**Single-line comments**

```c
// comment
```

**Multi-line comments**

```c
/* comment */
```

* Nested comments are not allowed
* Unclosed block comments are detected as errors

---

### 🔹 Error Handling (Lexer)

The lexer reports:

* Unclosed string literals
* Newline inside string literal
* Unclosed block comments
* Unknown / illegal characters

**Error format**

```
ERROR(<message>) line <number>
```

---

### 🔹 Parser

The parser consumes tokens produced by the lexer and validates them against the **SimpleLang grammar**.

#### Parser Responsibilities

* Validates statement structure
* Ensures correct token order
* Detects syntax errors early
* Prepares the project for AST generation

#### Supported Constructs

* Variable assignments
* Arithmetic expressions
* `if / else` statements
* `while` loops
* `return` statements
* Block scopes `{ ... }`

The parser follows a **top-down / recursive-descent** design.

---

### 🔹 Error Handling (Parser)

The parser reports:

* Unexpected tokens
* Missing symbols (`;`, `)`, `}` , etc.)
* Invalid statement forms

**Error format**

```
SYNTAX_ERROR(<message>) line <number>
```

---

## 📘 Project Phases

### Phase 1 — Lexer ✅ (Completed)

* Tokenization
* Comment removal
* String validation
* Token stream generation

### Phase 2 — Parser ✅ (Completed)

* Grammar definition
* Recursive-descent parsing
* Syntax validation



---

## 🔧 Build Instructions

### Linux & macOS

```bash
gcc -std=c99 lexer.c parser.c main.c -o simplelang
./simplelang test/test1.sl
```

### Windows (MinGW / w64devkit)

```bash
gcc -std=c99 lexer.c parser.c main.c -o simplelang.exe
simplelang.exe test\test1.sl
```

---

## 🧪 Example

### Input (`test1.sl`)

```c
if (a)
    if (b)
        x = 1;
    else
        x = 2;

```

### Output (Lexer Tokens)

```
IF
LPAREN
IDENTIFIER(a)
RPAREN
IF
LPAREN
IDENTIFIER(b)
RPAREN
IDENTIFIER(x)
ASSIGN
NUMBER(1)
SEMICOLON
ELSE
IDENTIFIER(x)
ASSIGN
NUMBER(2)
SEMICOLON

```

### Parser Result

```
Program
  IfStmt
    Expr
      Term
        Factor
          Identifier
    IfStmt
      Expr
        Term
          Factor
            Identifier
      Assignment
        Expr
          Term
            Factor
              Number
      Assignment
        Expr
          Term
            Factor
              Number

```

---

## 🧱 Implementation Summary

* Character-by-character input using `fgetc`
* Longest-match strategy for operators
* Line number tracking for precise errors
* Lexer and parser separated into modules
* Clean EOF handling

---

## 📂 Suggested Project Structure

```
SimpleLang/
├── scanner.c
├── output/
│   └── ...
├── test/
│   └── ...
└── README.md
```

---

## 📄 License

MIT License

You are free to use, modify, and distribute this project for educational or personal purposes.

---

## 👨‍💻 Author

* **Mahdi Jahed**
* 📫 [mahdijahed56@gmail.com](mailto:mahdijahed56@gmail.com)
* 🐙 GitHub: [https://github.com/MahdiJDS](https://github.com/MahdiJDS)

---

> This project is intentionally designed to grow with you — from lexer, to parser, to a full compiler.
