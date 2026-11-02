#!/usr/bin/env python3
"""Golden-file test runner for assay.

Layout:
    tests/<suite>/args          one line of extra CLI flags for the whole suite
                                (e.g. "--tokens"). Optional.
    tests/<suite>/foo.assay     the program
    tests/<suite>/foo.expected  exact expected stdout
    tests/<suite>/foo.exit      expected exit code (optional, default 0)

Usage:
    python3 tests/run_tests.py [--bin build/assay] [-v] [filter]

    filter  only run tests whose path contains this substring, e.g. "strings"
    -v      also show passing tests

Exit status is 0 only if every test passes, so CI and `make test` can rely on it.
"""

import argparse
import difflib
import signal
import subprocess
import sys
from pathlib import Path

TESTS_DIR = Path(__file__).resolve().parent
TIMEOUT_SECONDS = 5  # an infinite loop in the scanner is a very common bug

GREEN, RED, DIM, RESET = "\033[32m", "\033[31m", "\033[2m", "\033[0m"


def color(text, code):
    return f"{code}{text}{RESET}" if sys.stdout.isatty() else text


def describe_exit(code):
    # A negative return code means the process was killed by a signal.
    if code < 0:
        try:
            return f"crashed with {signal.Signals(-code).name}"
        except ValueError:
            return f"killed by signal {-code}"
    return f"exit code {code}"


def run_one(binary, test_file):
    suite_args_file = test_file.parent / "args"
    extra = suite_args_file.read_text().split() if suite_args_file.exists() else []

    expected_out = test_file.with_suffix(".expected").read_text()
    exit_file = test_file.with_suffix(".exit")
    expected_code = int(exit_file.read_text().strip()) if exit_file.exists() else 0

    try:
        proc = subprocess.run(
            [str(binary), *extra, str(test_file)],
            capture_output=True, text=True, timeout=TIMEOUT_SECONDS,
        )
    except subprocess.TimeoutExpired:
        return [f"timed out after {TIMEOUT_SECONDS}s (infinite loop?)"]

    problems = []
    if proc.returncode != expected_code:
        problems.append(f"expected exit code {expected_code}, got "
                        f"{describe_exit(proc.returncode)}")
    if proc.stdout != expected_out:
        diff = difflib.unified_diff(
            expected_out.splitlines(), proc.stdout.splitlines(),
            fromfile="expected", tofile="actual", lineterm="", n=2,
        )
        problems.append("stdout differs:\n    " + "\n    ".join(diff))
    if problems and proc.stderr.strip():
        problems.append("stderr:\n    " + proc.stderr.strip().replace("\n", "\n    "))
    return problems


def main():
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--bin", default=str(TESTS_DIR.parent / "build" / "assay"))
    parser.add_argument("-v", "--verbose", action="store_true")
    parser.add_argument("filter", nargs="?", default="")
    opts = parser.parse_args()

    binary = Path(opts.bin)
    if not binary.exists():
        print(f"error: {binary} not found. Run `make` first.")
        return 2

    tests = sorted(p for p in TESTS_DIR.rglob("*.assay") if opts.filter in str(p))
    if not tests:
        print("no tests matched")
        return 2

    failed = 0
    for test in tests:
        name = test.relative_to(TESTS_DIR).with_suffix("")
        if not test.with_suffix(".expected").exists():
            print(color(f"SKIP {name}", DIM), "(no .expected file)")
            continue
        problems = run_one(binary, test)
        if problems:
            failed += 1
            print(color(f"FAIL {name}", RED))
            for p in problems:
                print("  " + p)
        elif opts.verbose:
            print(color(f"pass {name}", GREEN))

    passed = len(tests) - failed
    summary = f"\n{passed}/{len(tests)} golden tests passed"
    print(color(summary, GREEN if failed == 0 else RED))
    return 0 if failed == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
