#ifndef assay_test_h
#define assay_test_h

// A deliberately tiny unit-test framework (~70 lines). Read it all; it's a
// good tour of the C preprocessor:
//   #cond        turns an expression into a string literal ("a == b")
//   __FILE__/__LINE__  give the location of the failing check
//   do { } while (0)   makes a multi-statement macro behave like one statement
//
// Note EXPECT_INT_EQ prints BOTH sides on failure. That's the same idea as
// the `expect a == b` feature planned for the language itself.

#include <stdio.h>
#include <string.h>

static int test_total = 0;
static int test_failed = 0;
static int test_current_failed = 0;

#define TEST(name) static void name(void)

#define RUN_TEST(name)                                       \
  do {                                                       \
    test_current_failed = 0;                                 \
    test_total++;                                            \
    name();                                                  \
    if (test_current_failed) {                               \
      test_failed++;                                         \
      printf("FAIL %s\n", #name);                            \
    } else {                                                 \
      printf("pass %s\n", #name);                            \
    }                                                        \
  } while (0)

#define FAIL_AT(...)                                         \
  do {                                                       \
    printf("  %s:%d: ", __FILE__, __LINE__);                 \
    printf(__VA_ARGS__);                                     \
    printf("\n");                                            \
    test_current_failed = 1;                                 \
  } while (0)

#define EXPECT(cond)                                         \
  do {                                                       \
    if (!(cond)) FAIL_AT("expected %s", #cond);              \
  } while (0)

#define EXPECT_INT_EQ(actual, expected)                      \
  do {                                                       \
    long long a_ = (long long)(actual);                      \
    long long e_ = (long long)(expected);                    \
    if (a_ != e_)                                            \
      FAIL_AT("expected %s == %s, got %lld vs %lld",         \
              #actual, #expected, a_, e_);                   \
  } while (0)

// Compare a (pointer, length) slice against a C string.
#define EXPECT_SLICE_EQ(ptr, len, expected)                  \
  do {                                                       \
    const char* e_ = (expected);                             \
    int l_ = (int)(len);                                     \
    if (l_ != (int)strlen(e_) || memcmp((ptr), e_, l_) != 0) \
      FAIL_AT("expected '%s', got '%.*s'", e_, l_, (ptr));   \
  } while (0)

#define TEST_SUMMARY()                                       \
  (printf("\n%d/%d unit tests passed\n",                     \
          test_total - test_failed, test_total),             \
   test_failed == 0 ? 0 : 1)

#endif
