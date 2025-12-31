#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_BUFFER 256

/*  لیست کلمات کلیدی  */
static const char *KEYWORD_LIST[] = {
    "if", "else", "while", "return", NULL};

/*  انواع توکن‌ها */
typedef enum
{
    TOK_IF,
    TOK_ELSE,
    TOK_WHILE,
    TOK_RETURN,
    TOK_IDENTIFIER,
    TOK_NUMBER,
    TOK_STRING,

    TOK_PLUS,
    TOK_MINUS,
    TOK_MULTIPLY,
    TOK_DIVIDE,

    TOK_ASSIGN,
    TOK_EQUAL,
    TOK_NOT_EQUAL,
    TOK_LESS_EQUAL,
    TOK_GREATER_EQUAL,
    TOK_LESS,
    TOK_GREATER,

    TOK_LPAREN,
    TOK_RPAREN,
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_SEMICOLON,
    TOK_COMMA,

    TOK_FINISHED,
    TOK_ERROR
} TokenType;

/*  ساختار توکن */
typedef struct
{
    TokenType type;
    char text[MAX_BUFFER];
    int line;
} Token;

/*  متغیرهای سراسری */
static FILE *inputFile = NULL;
static FILE *outputFile = NULL;
static int currentChar = 0;
static int currentLine = 1;

/*  توابع پایه */
static void readNextChar() { currentChar = fgetc(inputFile); }

static Token makeToken(TokenType type, const char *txt)
{
    Token t;
    t.type = type;
    if (txt)
        strncpy(t.text, txt, MAX_BUFFER);
    else
        t.text[0] = '\0';
    t.line = currentLine;
    return t;
}

static int isKeyword(const char *word)
{
    for (int i = 0; KEYWORD_LIST[i]; i++)
        if (strcmp(KEYWORD_LIST[i], word) == 0)
            return 1;
    return 0;
}

static void printToken(Token t)
{
    switch (t.type)
    {
    case TOK_IF:
        fprintf(outputFile, "IF\n");
        break;
    case TOK_ELSE:
        fprintf(outputFile, "ELSE\n");
        break;
    case TOK_WHILE:
        fprintf(outputFile, "WHILE\n");
        break;
    case TOK_RETURN:
        fprintf(outputFile, "RETURN\n");
        break;

    case TOK_IDENTIFIER:
        fprintf(outputFile, "IDENTIFIER(%s)\n", t.text);
        break;
    case TOK_NUMBER:
        fprintf(outputFile, "NUMBER(%s)\n", t.text);
        break;
    case TOK_STRING:
        fprintf(outputFile, "STRING(%s)\n", t.text);
        break;

    case TOK_PLUS:
        fprintf(outputFile, "PLUS\n");
        break;
    case TOK_MINUS:
        fprintf(outputFile, "MINUS\n");
        break;
    case TOK_MULTIPLY:
        fprintf(outputFile, "MUL\n");
        break;
    case TOK_DIVIDE:
        fprintf(outputFile, "DIV\n");
        break;

    case TOK_ASSIGN:
        fprintf(outputFile, "ASSIGN\n");
        break;
    case TOK_EQUAL:
        fprintf(outputFile, "EQ\n");
        break;
    case TOK_NOT_EQUAL:
        fprintf(outputFile, "NEQ\n");
        break;

    case TOK_LESS_EQUAL:
        fprintf(outputFile, "LEQ\n");
        break;
    case TOK_GREATER_EQUAL:
        fprintf(outputFile, "GEQ\n");
        break;
    case TOK_LESS:
        fprintf(outputFile, "LT\n");
        break;
    case TOK_GREATER:
        fprintf(outputFile, "GT\n");
        break;

    case TOK_LPAREN:
        fprintf(outputFile, "LPAREN\n");
        break;
    case TOK_RPAREN:
        fprintf(outputFile, "RPAREN\n");
        break;
    case TOK_LBRACE:
        fprintf(outputFile, "LBRACE\n");
        break;
    case TOK_RBRACE:
        fprintf(outputFile, "RBRACE\n");
        break;
    case TOK_SEMICOLON:
        fprintf(outputFile, "SEMICOLON\n");
        break;
    case TOK_COMMA:
        fprintf(outputFile, "COMMA\n");
        break;

    case TOK_ERROR:
        fprintf(outputFile, "ERROR(%s) line %d\n", t.text, t.line);
        break;

    default:
        break;
    }
}

/*  پرش فاصله‌ها  */
static void skipWhitespace()
{
    while (isspace(currentChar))
    {
        if (currentChar == '\n')
            currentLine++;
        readNextChar();
    }
}

/*  رد شدن از کامنت چندخطی  */
static int skipBlockComment()
{
    readNextChar();
    while (1)
    {
        if (currentChar == EOF)
            return 0;
        if (currentChar == '\n')
            currentLine++;

        if (currentChar == '*')
        {
            readNextChar();
            if (currentChar == '/')
            {
                readNextChar();
                return 1;
            }
        }
        else
            readNextChar();
    }
}

//   توابع ساخت توکن‌ها

// متغییر
static Token readIdentifierToken()
{
    char buf[MAX_BUFFER] = {0};
    int i = 0;

    while (isalnum(currentChar) || currentChar == '_')
    {
        buf[i++] = currentChar;
        readNextChar();
    }

    if (isKeyword(buf))
    {
        if (strcmp(buf, "if") == 0)
            return makeToken(TOK_IF, NULL);
        if (strcmp(buf, "else") == 0)
            return makeToken(TOK_ELSE, NULL);
        if (strcmp(buf, "while") == 0)
            return makeToken(TOK_WHILE, NULL);
        if (strcmp(buf, "return") == 0)
            return makeToken(TOK_RETURN, NULL);
    }

    return makeToken(TOK_IDENTIFIER, buf);
}

// عدد
static Token readNumberToken()
{
    char buf[MAX_BUFFER] = {0};
    int i = 0;

    while (isdigit(currentChar))
    {
        buf[i++] = currentChar;
        readNextChar();
    }
    return makeToken(TOK_NUMBER, buf);
}

// رشته
static Token readStringToken()
{
    char buf[MAX_BUFFER] = {0};
    int i = 0;

    readNextChar(); // skip "

    while (1)
    {
        if (currentChar == EOF)
            return makeToken(TOK_ERROR, "Unclosed string");

        if (currentChar == '\n')
            return makeToken(TOK_ERROR, "Unclosed string");

        if (currentChar == '\\')
        {
            readNextChar();
            if (currentChar == EOF)
                return makeToken(TOK_ERROR, "Bad escape");

            buf[i++] = '\\';
            buf[i++] = currentChar;
            readNextChar();
            continue;
        }

        if (currentChar == '"')
        {
            readNextChar();
            buf[i] = '\0';
            return makeToken(TOK_STRING, buf);
        }

        buf[i++] = currentChar;
        readNextChar();
    }
}

// عملگر ها
static Token readOperatorToken(int c)
{
    switch (c)
    {
    case '+':
        return makeToken(TOK_PLUS, NULL);
    case '-':
        return makeToken(TOK_MINUS, NULL);
    case '*':
        return makeToken(TOK_MULTIPLY, NULL);

    case '/':
        if (currentChar == '/')
        {
            while (currentChar != '\n' && currentChar != EOF)
                readNextChar();
            return (Token){0};
        }
        if (currentChar == '*')
        {
            if (!skipBlockComment())
                return makeToken(TOK_ERROR, "Comment not closed");
            return (Token){0};
        }
        return makeToken(TOK_DIVIDE, NULL);

    case '=':
        if (currentChar == '=')
        {
            readNextChar();
            return makeToken(TOK_EQUAL, NULL);
        }
        return makeToken(TOK_ASSIGN, NULL);

    case '!':
        if (currentChar == '=')
        {
            readNextChar();
            return makeToken(TOK_NOT_EQUAL, NULL);
        }
        return makeToken(TOK_ERROR, "Unexpected !");

    case '<':
        if (currentChar == '=')
        {
            readNextChar();
            return makeToken(TOK_LESS_EQUAL, NULL);
        }
        return makeToken(TOK_LESS, NULL);

    case '>':
        if (currentChar == '=')
        {
            readNextChar();
            return makeToken(TOK_GREATER_EQUAL, NULL);
        }
        return makeToken(TOK_GREATER, NULL);

    case '(':
        return makeToken(TOK_LPAREN, NULL);
    case ')':
        return makeToken(TOK_RPAREN, NULL);
    case '{':
        return makeToken(TOK_LBRACE, NULL);
    case '}':
        return makeToken(TOK_RBRACE, NULL);
    case ';':
        return makeToken(TOK_SEMICOLON, NULL);
    case ',':
        return makeToken(TOK_COMMA, NULL);

    default:
    {
        char buf[32];
        snprintf(buf, sizeof(buf), "Unknown '%c'", c);
        return makeToken(TOK_ERROR, buf);
    }
    }
}

//   گرفتن توکن

static Token nextToken()
{
    skipWhitespace();

    if (currentChar == EOF)
        return makeToken(TOK_FINISHED, "");

    if (isalpha(currentChar) || currentChar == '_')
        return readIdentifierToken();

    if (isdigit(currentChar))
        return readNumberToken();

    if (currentChar == '"')
        return readStringToken();

    int c = currentChar;
    readNextChar();
    Token t = readOperatorToken(c);

    if (t.type == 0) // یعنی دوباره باید صدا بزنیم
        return nextToken();

    return t;
}

/* ===================== PARSER ===================== */

static Token lookahead;
static int indent = 0;

void printIndent()
{
    for (int i = 0; i < indent; i++)
        printf("  ");
}

void advance()
{
    lookahead = nextToken();
}

void error(const char *msg)
{
    printf("Syntax Error (line %d): %s\n", lookahead.line, msg);
    advance();
}

void match(TokenType t)
{
    if (lookahead.type == t)
        advance();
    else
        error("Unexpected token");
}


void parseStmtList();
void parseStmt();
void parseExpr();
void parseExprPrime();
void parseTerm();
void parseTermPrime();
void parseFactor();


void parseProgram()
{
    printIndent();
    printf("Program\n");
    indent++;
    parseStmtList();
    indent--;
}

void parseStmtList()
{
    if (lookahead.type == TOK_IDENTIFIER ||
        lookahead.type == TOK_IF ||
        lookahead.type == TOK_LBRACE)
    {
        parseStmt();
        parseStmtList();
    }
}

void parseStmt()
{
    if (lookahead.type == TOK_IDENTIFIER)
    {
        printIndent();
        printf("Assignment\n");
        indent++;
        match(TOK_IDENTIFIER);
        match(TOK_ASSIGN);
        parseExpr();
        match(TOK_SEMICOLON);
        indent--;
    }
    else if (lookahead.type == TOK_IF)
    {
        printIndent();
        printf("IfStmt\n");
        indent++;
        match(TOK_IF);
        match(TOK_LPAREN);
        parseExpr();
        match(TOK_RPAREN);
        parseStmt();
        if (lookahead.type == TOK_ELSE)
        {
            match(TOK_ELSE);
            parseStmt();
        }
        indent--;
    }
    else if (lookahead.type == TOK_LBRACE)
    {
        printIndent();
        printf("Block\n");
        indent++;
        match(TOK_LBRACE);
        parseStmtList();
        match(TOK_RBRACE);
        indent--;
    }
    else
    {
        error("Invalid statement");
    }
}

void parseExpr()
{
    printIndent();
    printf("Expr\n");
    indent++;
    parseTerm();
    parseExprPrime();
    indent--;
}

void parseExprPrime()
{
    if (lookahead.type == TOK_PLUS || lookahead.type == TOK_MINUS)
    {
        printIndent();
        printf("Expr'\n");
        indent++;

        printIndent();
        printf("%s\n", lookahead.type == TOK_PLUS ? "+" : "-");
        match(lookahead.type);

        parseTerm();
        parseExprPrime();

        indent--;
    }
}

void parseTerm()
{
    printIndent();
    printf("Term\n");
    indent++;
    parseFactor();
    parseTermPrime();
    indent--;
}

void parseTermPrime()
{
    if (lookahead.type == TOK_MULTIPLY || lookahead.type == TOK_DIVIDE)
    {
        printIndent();
        printf("Term'\n");
        indent++;

        printIndent();
        printf("%s\n", lookahead.type == TOK_MULTIPLY ? "*" : "/");
        match(lookahead.type);

        parseFactor();
        parseTermPrime();

        indent--;
    }
}

void parseFactor()
{
    printIndent();
    printf("Factor\n");
    indent++;

    if (lookahead.type == TOK_NUMBER)
    {
        printIndent();
        printf("Number\n");
        match(TOK_NUMBER);
    }
    else if (lookahead.type == TOK_IDENTIFIER)
    {
        printIndent();
        printf("Identifier\n");
        match(TOK_IDENTIFIER);
    }
    else if (lookahead.type == TOK_LPAREN)
    {
        match(TOK_LPAREN);
        parseExpr();
        match(TOK_RPAREN);
    }
    else
    {
        error("Invalid factor");
    }

    indent--;
}

// MAIN

int main(int argc, char **argv)
{
    FILE *outputFile;

    if (argc < 3)
    {
        printf("Usage: compiler <input_file> <output_file>\n");
        return 1;
    }

    inputFile = fopen(argv[1], "r");
    if (!inputFile)
    {
        printf("Error: cannot open input file\n");
        return 1;
    }

    outputFile = fopen(argv[2], "w");
    if (!outputFile)
    {
        printf("Error: cannot open output file\n");
        fclose(inputFile);
        return 1;
    }

    freopen(argv[2], "w", stdout);

    readNextChar();
    advance();     
    parseProgram(); 

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
