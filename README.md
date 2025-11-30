# SimpleLang Lexer / Scanner  
*A minimal lexical analyzer written in C for the SimpleLang educational language.*

---

## 📌 Overview
This project implements a **lexer (scanner)** for a simplified programming language called **SimpleLang**, commonly used in compiler design courses.  
The lexer reads a source file, breaks it into tokens, handles errors, removes comments, and writes the final token stream into an output file.

This project is ideal for:
- Compiler design assignments  
- Teaching lexical analysis  
- Extending into a full compiler pipeline (parser → AST → interpreter)

---

## 🚀 Features
- Fully written in **ANSI C / C99**
- Supports:
  - Keywords: `if`, `else`, `while`, `return`
  - Identifiers (`[A-Za-z_][A-Za-z0-9_]*`)
  - Integers (`[0-9]+`)
  - String literals with escape support (`\"`, `\\`)
- Multi-character operators:
  - `==`, `!=`, `<=`, `>=`
- Single-character operators:
  - `+`, `-`, `*`, `/`
- Separators:
  - `(` `)` `{` `}` `,` `;`
- Comment handling:
  - Single-line: `// comment`
  - Multi-line: `/* comment */`
- Error detection for:
  - Unclosed strings
  - Unclosed block comments
  - Illegal characters
- Generates a clean token output file.

---

## 📘 Project Phases

### Phase 1 — Lexer ✔️ (Completed)
- Tokenization
- Comment handling
- String validation
- Output token stream

### Phase 2 — Parser / AST ⏳ (Coming Soon)
- Grammar definition
- AST generation
- Syntax validation

### Phase 3 — Semantic Analysis / Code Generation ⏳ (Planned)
- Symbol tables
- Scope & type checking
- Intermediate representation (IR)
- Optional: code generation backend

---

## 🔧 Build Instructions

### Linux & macOS
```bash
gcc -std=c99 scanner.c -o scanner
./scanner test/test1.sl

```

### Windows (MinGW / w64devkit)
```bash
gcc -std=c99 scanner.c -o scanner.exe
scanner.exe test\test1.sl

```

---

## 📤 Output Format

Each token is printed on a new line in the output file.

### Examples:

IF
WHILE
IDENTIFIER(x1)
NUMBER(10)
STRING(hello)
PLUS
ASSIGN
GEQ
SEMICOLON


### Error example:

ERROR(Unclosed string) line 5

🧪 Example Input / Output
Input
while(x1 >= 10) {
    x1 = x1 - 1;
    if (x1 == 5) {
        return;
    }
}

### Output
WHILE
LPAREN
IDENTIFIER(x1)
GEQ
NUMBER(10)
RPAREN
LBRACE
IDENTIFIER(x1)
ASSIGN
IDENTIFIER(x1)
MINUS
NUMBER(1)
SEMICOLON
IF
LPAREN
IDENTIFIER(x1)
EQ
NUMBER(5)
RPAREN
LBRACE
RETURN
SEMICOLON
RBRACE
RBRACE

---

## 📝 Token Rules
Identifiers
[A-Za-z_][A-Za-z0-9_]*

Numbers
[0-9]+

String Literals
"([^"\\]|\\.)*"


### Rules:

No newline allowed inside quotes

Escape sequences supported: \", \\

Comments

Single-line:

    // comment


Multi-line:

    /* comment */

Operators
Type	Tokens
Arithmetic	+ - * /
Assignment	=
Comparison	== != <= >= < >

--- 

## ⚠️ Error Handling

### The lexer reports:

Unclosed string

Unclosed block comment

Unexpected newline inside string

Unknown characters

Error format:

ERROR(<message>) line <number>

---


## 🧱 Implementation Summary

Reads input character-by-character using fgetc

Skips whitespace and comments

Performs longest-match on multi-character operators

Tracks line numbers for accurate error reporting

Writes tokens into an output text file

Stops cleanly on EOF



## 📄 License

You may use this project under the MIT License.
If you want, I can generate the LICENSE file automatically.

---

## 👨‍💻 Author

- 📫 mahdijahed56@gamil.com
- 🐙 [GitHub](https://github.com/MahdiJDS) 

---