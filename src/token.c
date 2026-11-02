#include "scanner.h"

// The second use of the X-macro: turn each token name into a string.
// "#name" is the preprocessor's stringizing operator.
static const char* const TOKEN_NAMES[] = {
#define X(name) #name,
  TOKEN_LIST(X)
#undef X
};

const char* tokenTypeName(TokenType type) {
  int count = (int)(sizeof(TOKEN_NAMES) / sizeof(TOKEN_NAMES[0]));
  if ((int)type < 0 || (int)type >= count) return "???";
  return TOKEN_NAMES[type];
}
