// Unit tests for the scanner. These check things the golden tests can't see
// easily: exact token fields, one token at a time.
//
// Rule for yourself: when you find a scanner bug, write a failing test here
// FIRST, then fix it. That's the habit SDET/SWE interviewers ask about.

#include "../../src/scanner.h"
#include "test.h"

// Helper: scan the next token and check its type and lexeme.
#define EXPECT_TOKEN(type_, lexeme_)                  \
  do {                                                \
    Token t_ = scanToken();                           \
    if (t_.type != (type_))                           \
      FAIL_AT("expected %s, got %s ('%.*s')",         \
              tokenTypeName(type_),                   \
              tokenTypeName(t_.type), t_.length,      \
              t_.start);                              \
    else                                              \
      EXPECT_SLICE_EQ(t_.start, t_.length, lexeme_);  \
  } while (0)

TEST(test_empty_source) {
  initScanner("");
  Token t = scanToken();
  EXPECT_INT_EQ(t.type, TOKEN_EOF);
  EXPECT_INT_EQ(t.line, 1);
  // Asking again after EOF must keep returning EOF, not read past the end.
  EXPECT_INT_EQ(scanToken().type, TOKEN_EOF);
}

TEST(test_single_char_tokens) {
  initScanner("(){}[],.-+;/*%");
  EXPECT_TOKEN(TOKEN_LEFT_PAREN, "(");
  EXPECT_TOKEN(TOKEN_RIGHT_PAREN, ")");
  EXPECT_TOKEN(TOKEN_LEFT_BRACE, "{");
  EXPECT_TOKEN(TOKEN_RIGHT_BRACE, "}");
  EXPECT_TOKEN(TOKEN_LEFT_BRACKET, "[");
  EXPECT_TOKEN(TOKEN_RIGHT_BRACKET, "]");
  EXPECT_TOKEN(TOKEN_COMMA, ",");
  EXPECT_TOKEN(TOKEN_DOT, ".");
  EXPECT_TOKEN(TOKEN_MINUS, "-");
  EXPECT_TOKEN(TOKEN_PLUS, "+");
  EXPECT_TOKEN(TOKEN_SEMICOLON, ";");
  EXPECT_TOKEN(TOKEN_SLASH, "/");
  EXPECT_TOKEN(TOKEN_STAR, "*");
  EXPECT_TOKEN(TOKEN_PERCENT, "%");
  EXPECT_TOKEN(TOKEN_EOF, "");
}

TEST(test_two_char_operators) {
  // No spaces: the scanner must still split these correctly ("maximal munch").
  initScanner("!!====<<=>>=");
  EXPECT_TOKEN(TOKEN_BANG, "!");
  EXPECT_TOKEN(TOKEN_BANG_EQUAL, "!=");
  EXPECT_TOKEN(TOKEN_EQUAL_EQUAL, "==");
  EXPECT_TOKEN(TOKEN_EQUAL, "=");
  EXPECT_TOKEN(TOKEN_LESS, "<");
  EXPECT_TOKEN(TOKEN_LESS_EQUAL, "<=");
  EXPECT_TOKEN(TOKEN_GREATER, ">");
  EXPECT_TOKEN(TOKEN_GREATER_EQUAL, ">=");
  EXPECT_TOKEN(TOKEN_EOF, "");
}

TEST(test_line_numbers) {
  initScanner("a\nb\n\n  c // comment with ( tokens ) inside\nd");
  EXPECT_INT_EQ(scanToken().line, 1);
  EXPECT_INT_EQ(scanToken().line, 2);
  EXPECT_INT_EQ(scanToken().line, 4);
  Token d = scanToken();
  EXPECT_INT_EQ(d.type, TOKEN_IDENTIFIER);
  EXPECT_INT_EQ(d.line, 5);
  EXPECT_INT_EQ(scanToken().type, TOKEN_EOF);
}

TEST(test_strings) {
  initScanner("\"hi\" \"\"");
  EXPECT_TOKEN(TOKEN_STRING, "\"hi\"");  // lexeme keeps the quotes
  EXPECT_TOKEN(TOKEN_STRING, "\"\"");
  EXPECT_TOKEN(TOKEN_EOF, "");
}

TEST(test_multiline_string_reports_start_line) {
  // Design decision (differs from clox): a token's line is where it STARTS.
  // Error messages pointing at the start of a string are far more useful.
  initScanner("x\n\"one\ntwo\nthree\" y");
  EXPECT_INT_EQ(scanToken().line, 1);  // x
  Token str = scanToken();
  EXPECT_INT_EQ(str.type, TOKEN_STRING);
  EXPECT_INT_EQ(str.line, 2);
  EXPECT_INT_EQ(scanToken().line, 4);  // y: lines inside the string still count
}

TEST(test_numbers) {
  initScanner("0 42 3.14 7.");
  EXPECT_TOKEN(TOKEN_NUMBER, "0");
  EXPECT_TOKEN(TOKEN_NUMBER, "42");
  EXPECT_TOKEN(TOKEN_NUMBER, "3.14");
  EXPECT_TOKEN(TOKEN_NUMBER, "7");  // trailing dot is NOT part of the number
  EXPECT_TOKEN(TOKEN_DOT, ".");
  EXPECT_TOKEN(TOKEN_EOF, "");
}

TEST(test_keywords) {
  initScanner("fn let test expect letter fnord testing _if if");
  EXPECT_TOKEN(TOKEN_FN, "fn");
  EXPECT_TOKEN(TOKEN_LET, "let");
  EXPECT_TOKEN(TOKEN_TEST, "test");
  EXPECT_TOKEN(TOKEN_EXPECT, "expect");
  // Prefixes and extensions of keywords are plain identifiers.
  EXPECT_TOKEN(TOKEN_IDENTIFIER, "letter");
  EXPECT_TOKEN(TOKEN_IDENTIFIER, "fnord");
  EXPECT_TOKEN(TOKEN_IDENTIFIER, "testing");
  EXPECT_TOKEN(TOKEN_IDENTIFIER, "_if");
  EXPECT_TOKEN(TOKEN_IF, "if");
  EXPECT_TOKEN(TOKEN_EOF, "");
}

TEST(test_errors) {
  initScanner("@ \"never closed");
  Token bad = scanToken();
  EXPECT_INT_EQ(bad.type, TOKEN_ERROR);
  EXPECT_SLICE_EQ(bad.start, bad.length, "Unexpected character.");
  Token str = scanToken();
  EXPECT_INT_EQ(str.type, TOKEN_ERROR);
  EXPECT_SLICE_EQ(str.start, str.length, "Unterminated string.");
  EXPECT_INT_EQ(scanToken().type, TOKEN_EOF);
}

int main(void) {
  RUN_TEST(test_empty_source);
  RUN_TEST(test_single_char_tokens);
  RUN_TEST(test_two_char_operators);
  RUN_TEST(test_line_numbers);
  RUN_TEST(test_strings);
  RUN_TEST(test_multiline_string_reports_start_line);
  RUN_TEST(test_numbers);
  RUN_TEST(test_keywords);
  RUN_TEST(test_errors);
  return TEST_SUMMARY();
}
