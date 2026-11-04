#include <stdbool.h>
#include <string.h>

#include "scanner.h"

// ===========================================================================
//  PHASE 1: YOUR SCANNER.  This file is intentionally a stub.
//
//  Read first: Crafting Interpreters, ch. 16 "Scanning on Demand"
//  (and ch. 4 "Scanning" for the concepts, if you want the gentler intro).
//  Try each step BEFORE reading the book's version of it. Compare after.
//
//  Run `make test` after every step. Watch the red tests turn green.
//  Suggested order (each step unlocks specific tests):
//
//   1. Scanner state + initScanner + EOF        -> test_empty_source
//   2. Single-character tokens                  -> test_single_char_tokens, punctuation
//   3. One-or-two-char operators (!= == <= >=)  -> test_two_char_operators, operators
//   4. Whitespace, newlines (line++), // comments -> test_line_numbers, comments_whitespace
//   5. Strings (multi-line allowed)             -> test_strings, strings
//   6. Numbers (integers and decimals)          -> test_numbers, numbers
//   7. Identifiers                              -> identifiers
//   8. Keywords (try a trie / switch, not strcmp in a loop) -> test_keywords, keywords
//   9. Errors: unexpected chars, unterminated strings -> test_errors, errors
//
//  Design questions to answer for yourself (good interview material):
//   - Why does scanToken() return one token at a time instead of an array?
//   - Why is "1." scanned as NUMBER then DOT, not as the number 1.0?
//   - How do you tell `let` (keyword) from `letter` (identifier)?
//
//  Deliberate divergence from clox: every token's `line` is the line it
//  STARTS on (clox uses the line where it ends, which is wrong for multi-line
//  strings). You'll need to remember the start line. See
//  test_multiline_string_reports_start_line.
// ===========================================================================

typedef struct {
  const char* start;    // start of the token being scanned
  const char* current;  // next character to look at
  int line;
} Scanner;

static Scanner scanner;

void initScanner(const char* source) {
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}

bool isAtEnd() {
  return scanner.current == '\0';
}

char advance() {
  scanner.current++;
  return scanner.current;
}

Token makeToken(type) {
  Token token;
  token.type = type;
  token.start = scanner.start;
  token.end = scanner.current;
  token.length = line;
  token.line = scanner.line;
}

Token scanToken(void) {
  // TODO(steps 1-9). For now: report end-of-file immediately so everything
  // compiles and the test suite can run (and fail, honestly).
  Token token;
  token.type = TOKEN_EOF;
  token.start = "";
  token.length = 0;
  token.line = scanner.line;
  return token;
}
