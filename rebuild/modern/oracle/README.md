# `oracle/` — retail behavioural oracle (optional developer tool)

Runs functions of the user's own `Fable.exe` inside [Unicorn](https://github.com/unicorn-engine/unicorn)
so portable reconstructions in `../core` (and later modules) can be compared on identical inputs. It
is built only when Unicorn is installed, is never linked into the port, and embeds no retail bytes.

```sh
# Unicorn from source (x86 only is enough):
git clone --depth 1 --branch 2.1.3 https://github.com/unicorn-engine/unicorn
cmake -S unicorn -B unicorn/build -DUNICORN_ARCH=x86 -DUNICORN_BUILD_TESTS=OFF
cmake --build unicorn/build && cmake --install unicorn/build --prefix <prefix>

cmake -S rebuild/modern -B build/modern -DCMAKE_PREFIX_PATH=<prefix>
cmake --build build/modern
FABLE_EXE="<Fable The Lost Chapters>/Fable.exe" build/modern/oracle/fable_oracle_tests [filter]
```

`RetailOracle` maps the PE at 0x00400000, traps every import (a few CRT helpers are stubbed;
any other import call fails loudly), provides a flat-segment GDT with a minimal TEB, starts each
call after `fninit; fldcw 0x027F` (the MSVC 7.1 FPU mode), checks the callee's stack cleanup,
and reads results from EAX/EDX/ST0. `runUntil` executes a slice of a function (used to run
`Math_InitializeCosineLookup` up to its CRT call).
