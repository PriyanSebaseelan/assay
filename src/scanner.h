#ifndef assay_scanner_h
#define assay_scanner_h

// ---------------------------------------------------------------------------
// Token types, defined once with an "X-macro".
//
// TOKEN_LIST(X) expands X(name) for every token type. We use it twice:
//   1. in scanner.h to build the TokenType enum  (TOKEN_LEFT_PAREN, ...)
//   2. in token.c   to build the matching name table ("LEFT_PAREN", ...)
// Add a token here and both stay in sync automatically. This is a real
// technique used in production C codebases (the Linux kernel, CPython, LLVM).
// ---------------------------------------------------------------------------
#define TOKEN_LIST(X)                                                     \
  /* Single-character tokens. */                                          \
  X(LEFT_PAREN) X(RIGHT_PAREN) X(LEFT_BRACE) X(RIGHT_BRACE)               \
  X(LEFT_BRACKET) X(RIGHT_BRACKET)                                        \
  X(COMMA) X(DOT) X(MINUS) X(PLUS) X(SEMICOLON) X(SLASH) X(STAR)          \
  X(PERCENT)                                                              \
  /* One- or two-character tokens. */                                     \
  X(BANG) X(BANG_EQUAL) X(EQUAL) X(EQUAL_EQUAL)                           \
  X(GREATER) X(GREATER_EQUAL) X(LESS) X(LESS_EQUAL)                       \
  /* Literals. */                                                         \
  X(IDENTIFIER) X(STRING) X(NUMBER)                                       \
  /* Keywords. */                                                         \
  X(AND) X(ELSE) X(EXPECT) X(FALSE) X(FN) X(FOR) X(IF) X(LET) X(NIL)      \
  X(OR) X(PRINT) X(RETURN) X(TEST) X(TRUE) X(WHILE)                       \
  /* Special. */                                                          \
  X(ERROR) X(EOF)

typedef enum {
#define X(name) TOKEN_##name,
  TOKEN_LIST(X)
#undef X
} TokenType;

// A token does NOT own a copy of its text. It points into the source buffer
// (start + length). That makes scanning allocation-free, but it means the
// source string must outlive every token. Think about why that's fine here.
//
// For TOKEN_ERROR, start/length point at a static error message instead
// (e.g. "Unexpected character.").
typedef struct {
  TokenType type;
  const char* start;
  int length;
  int line;
} Token;

// Implemented in token.c (provided).
const char* tokenTypeName(TokenType type);

// Implemented in scanner.c (YOU write these).
void initScanner(const char* source);
Token scanToken(void);

#endif
