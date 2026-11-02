#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.h"

// Exit codes follow the BSD sysexits.h convention (same as clox):
//   64 = usage error, 65 = bad input (compile error), 70 = internal error,
//   74 = I/O error. Tests check these, so be consistent.
#define EXIT_USAGE 64
#define EXIT_DATAERR 65
#define EXIT_SOFTWARE 70
#define EXIT_IOERR 74

static char* readFile(const char* path) {
  FILE* file = fopen(path, "rb");
  if (file == NULL) {
    fprintf(stderr, "assay: could not open file \"%s\".\n", path);
    exit(EXIT_IOERR);
  }

  fseek(file, 0L, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);

  char* buffer = malloc((size_t)fileSize + 1);
  if (buffer == NULL) {
    fprintf(stderr, "assay: not enough memory to read \"%s\".\n", path);
    exit(EXIT_IOERR);
  }

  size_t bytesRead = fread(buffer, 1, (size_t)fileSize, file);
  if (bytesRead < (size_t)fileSize) {
    fprintf(stderr, "assay: could not read file \"%s\".\n", path);
    exit(EXIT_IOERR);
  }

  buffer[bytesRead] = '\0';
  fclose(file);
  return buffer;
}

// `assay --tokens file.assay` prints one token per line:
//   <line> <TYPE> '<lexeme>'
// The format is fixed because the golden tests in tests/scanner/ diff it.
static int dumpTokens(const char* source) {
  initScanner(source);
  int hadError = 0;
  for (;;) {
    Token token = scanToken();
    printf("%4d %-13s '%.*s'\n", token.line, tokenTypeName(token.type),
           token.length, token.start);
    if (token.type == TOKEN_ERROR) hadError = 1;
    if (token.type == TOKEN_EOF) break;
  }
  return hadError ? EXIT_DATAERR : 0;
}

static void usage(void) {
  fprintf(stderr, "Usage: assay [--tokens] <file.assay>\n");
  exit(EXIT_USAGE);
}

int main(int argc, const char* argv[]) {
  if (argc == 3 && strcmp(argv[1], "--tokens") == 0) {
    char* source = readFile(argv[2]);
    int status = dumpTokens(source);
    free(source);
    return status;
  }

  if (argc == 2 && argv[1][0] != '-') {
    // Phase 2+ goes here: compile(source) -> run on the VM.
    fprintf(stderr, "assay: running programs isn't implemented yet "
                    "(Phase 2). Try --tokens.\n");
    return EXIT_SOFTWARE;
  }

  usage();
  return EXIT_USAGE;
}
