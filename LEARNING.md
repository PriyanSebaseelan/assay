# How to learn while building this

The goal is to be able to explain every line in an interview. So the rule is
simple: **you write the language; the scaffolding exists to tell you whether
you got it right.**

## The loop, for every step

1. **Read the concept, not the code.** Read the relevant section of
   Crafting Interpreters up to where the code starts, then stop.
2. **Attempt it yourself, closed book.** 20–45 minutes. Being stuck is the
   learning; do not skip this.
3. **Run `make test`.** Read the failing diffs. Fix. Repeat.
4. **Then read the book's version.** Compare. Where it's better, understand
   *why* before you change yours. Never paste; retype only once you get it.
5. **Write two lines in `JOURNAL.md`.** What you built, and one thing that
   surprised you. In six months, this becomes your interview answers.
6. **Commit.** Small commits with real messages
   (`scanner: handle two-char operators`). Recruiters do read commit history.

## When you're stuck

In order: re-read the failing test → add a `printf` → step through with
`gdb ./build/test_scanner` (`break scanToken`, `next`, `print *scanner.current`)
→ re-read the book section → then ask for a *hint*, not a solution.

If you ask an AI for help, ask for a hint or a question back, not code.
"What case am I missing in my number scanner?" teaches you; "write my
number scanner" doesn't, and you won't be able to defend it in an interview.

## Who writes what

| Provided (scaffolding) | Yours (the language) |
|---|---|
| Makefile, CI, `.gitignore` | `scanner.c`, and later `compiler.c`, `vm.c`, `chunk.c`, … |
| `test.h`, `run_tests.py` | New tests for every phase and every bug you fix |
| Phase 1 tests (the spec) | All tests from Phase 2 onward |
| `main.c`, `token.c` | The design decisions (syntax, error messages, `expect`) |

Read the provided files too. `test.h` and the X-macro in `scanner.h` are each
a short lesson in C.

## Phase 2 onward: tests first

For each new feature, write the `.assay` program and its `.expected` output
*before* the implementation exists. Watch it fail, then make it pass. Writing the
expected output by hand forces you to decide exactly what the language does.
This is test-driven development, and it's the honest SDET story for your resume.
