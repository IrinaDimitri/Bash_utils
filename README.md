# Bash_utils

## Description

A learning project that reimplements two classic command-line utilities from the GNU Coreutils family — **`cat`** and **`grep`** — in **C (C11)**. The goal is to mirror familiar tool behaviour: line-by-line file output with numbering and non-printing character display options (`cat`), and POSIX regular expression search with a set of flags (`grep`).

**Stack:** C11, GCC, GNU Make, Bash (test scripts), clang-format (Google style).

---

## Table of contents

1. [Project layout](#project-layout)
2. [cat utility](#cat-utility)
3. [grep utility](#grep-utility)
4. [Build and run](#build-and-run)
5. [Testing](#testing)
6. [Code style checks](#code-style-checks)
7. [License](#license)

---

## Project layout

```text
Bash_utils
├── LICENSE
├── README.md
├── README_RUS.md
└── src/
    ├── cat/
    │   ├── Makefile          # build cat; test / style / clean targets
    │   ├── cat.c, cat.h
    │   ├── test_func_cat.sh  # functional tests
    │   ├── test_leak_cat.sh  # leak-check scenario (when valgrind is available)
    │   └── *.txt             # test data
    └── grep/
        ├── Makefile
        ├── grep.c, grep.h
        ├── test_func_grep.sh
        ├── test_leak_grep.sh
        └── *.txt
```

---

## cat utility

Copies the contents of given **files** to standard output according to the options (subset of classic `cat` behaviour).

### Supported options

| Option | Long form (where available) | Purpose |
|--------|-------------------------------|---------|
| `-b` | `--number-nonblank` | Number only non-empty lines |
| `-e` | — | Like `-v`, plus `$` at end of line |
| `-E` | — | Only `$` at end of line (without full `-v`) |
| `-n` | `--number` | Number all lines |
| `-s` | `--squeeze-blank` | Squeeze consecutive empty lines |
| `-t` | — | Show tabs as `^I`; `-t` also enables `-v` |
| `-T` | — | Only tab replacement as `^I` |
| `-v` | — | Show non-printing characters in a visible form |

Long options are handled via `getopt_long` (see `cat.c`).

### Example

```bash
cd src/cat
make
./cat -n test.txt
```

---

## grep utility

Searches **files** for lines matching a regular expression (POSIX `regcomp` / `regexec`). Multiple patterns are supported via repeated `-e`, and patterns can be loaded from a file with `-f`.

### Main options

| Option | Purpose |
|--------|---------|
| `-e <pattern>` | Set a pattern (repeatable, up to the limit defined in code) |
| `-f <file>` | Read patterns from a file |
| `-i` | Case-insensitive matching |
| `-v` | Invert match (lines that **do not** match) |
| `-c` | Print only the count of matching lines |
| `-l` | Print only filenames that contain a match |
| `-n` | Prefix lines with line numbers |
| `-h` | Do not print filenames |
| `-s` | Suppress error messages for failed file opens |
| `-o` | Print only the matching part of the line |

### Example

```bash
cd src/grep
make
./grep -n "pattern" test_1_grep.txt
```

---

## Build and run

### Requirements

- **GCC** with **C11** support.
- **GNU Make**.
- **Bash** for tests.
- **clang-format** for the `style` target (Google style check).
- **valgrind** for `test_leak_*.sh` scripts (if the script expects it).

### Building each utility

**cat:**

```bash
cd src/cat
make        # build the cat binary
```

**grep:**

```bash
cd src/grep
make        # build the grep binary
```

### Clean and rebuild

```bash
make clean           # remove objects and the binary
make clean && make   # full rebuild
```

A `rebuild` target exists in the Makefile; for a reliable rebuild, prefer `make clean && make`.

---

## Testing

Run functional tests via **Make**:

```bash
cd src/cat && make test
```

```bash
cd src/grep && make test
```

`test_func_cat.sh` and `test_func_grep.sh` compare behaviour against a reference. `test_leak_*.sh` scripts are for memory-leak checks when valgrind is installed.

---

## Code style checks

In each subproject:

```bash
make style
```

This runs `clang-format -style=Google -n *.c *.h` (check only; files are not rewritten automatically).

---

## License

The project is released under the **MIT License** — see the `LICENSE` file in the repository root.

---

## CI/CD

There are **no** GitHub Actions or other pipelines configured in this repository; build and tests are run locally with `make`.
