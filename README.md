# assay

A small dynamically typed scripting language with **built-in testing**, compiled to bytecode and run on a stack-based virtual machine. Written in C11 with no dependencies.

> **Status:** Phase 1 (scanner) in progress. This README only lists what actually works.

```
fn fib(n) {
  if (n < 2) { return n; }
  return fib(n - 1) + fib(n - 2);
}

test "fib base cases" {
  expect fib(0) == 0;
  expect fib(10) == 55;
}
```

## Build and test

```sh
make                # builds build/assay
make test           # unit tests + golden-file tests
make test SANITIZE=1    # same, under AddressSanitizer + UBSan
./build/assay --tokens tests/scanner/fib.assay
```

## Architecture

```
source ──▶ scanner ──▶ compiler ──▶ bytecode ──▶ VM
           (Phase 1)   (Phase 2+)               (Phase 2+)
```

| Path | What it is |
|---|---|
| `src/scanner.c` | Turns source text into tokens, one at a time, on demand |
| `src/token.c` | Token names, generated from one X-macro list in `scanner.h` |
| `src/main.c` | CLI entry point |
| `tests/unit/` | C unit tests and a ~70-line test framework (`test.h`) |
| `tests/<suite>/` | Golden tests: `foo.assay` + `foo.expected` (+ optional `foo.exit`) |
| `tests/run_tests.py` | Runs each golden test, diffs the output, catches crashes and timeouts |

## Roadmap

- [ ] Phase 1: scanner
- [ ] Phase 2: compiler and VM for expressions
- [ ] Phase 3: variables and statements
- [ ] Phase 4: control flow
- [ ] Phase 5: functions and call frames
- [ ] `test` / `expect` with failure messages that show both operands
- [ ] Stretch: strings, arrays, closures, garbage collection

## Acknowledgements

The architecture follows Robert Nystrom's [*Crafting Interpreters*](https://craftinginterpreters.com/) (Part III, clox). The syntax, the built-in `test`/`expect` feature, and the test infrastructure are this project's own work.
