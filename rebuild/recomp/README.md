# `recomp/` — static recompiler for Fable.exe (x86-32 → portable C)

Goal: run the **whole retail game logic** natively on x64 Windows, Linux and ARM64 Android, reading
all data from the user's own install, while readable decompiled code (`../modern`) replaces
recompiled functions over time. No retail bytes are committed: generated C is produced locally
from the user's `Fable.exe`.

## How it works

- **Lifter** (`lifter/`, C++20 + [Zydis](https://github.com/zyantific/zydis)): decodes from every
  entry in `rebuild/manifest/functions.tsv` plus every direct call target by recursive descent,
  resolves MSVC jump tables (immediate and register bounds, two-level byte-indexed tables, guarded
  fallback scan), and emits one C function per guest function.
- **Generated C** (`F_xxxxxxxx(Ctx*)`): GPRs and lazy flags live in C locals (spilled only at
  calls/returns/indirect branches) so the C compiler optimises freely; x87 is modelled in IEEE
  `double` with precision-control rounding (bit-exact under the MSVC 7.1 control word, see
  `../modern/core/README.md`); MMX/SSE/SSE2 via 128-bit lane temporaries. Guest memory is a flat
  4 GiB region (`g_mem`), PE image at its retail base 0x00400000, so guest pointers stay 32-bit.
- **Calls**: direct calls are C calls with the guest return address pushed, `ret N` pops it;
  tail jumps become calls + return; indirect calls/jumps (vtables, imports, function pointers)
  go through `recomp_dispatch` (binary search over all function entries, then host hooks for
  import thunks).
- **Runtime** (`runtime/`): `recomp.h` (context, memory accessors, flags, x87 helpers),
  `runtime.c` (guest memory, dispatch, CPUID/RDTSC, fatal hooks).

## Status (2026-10-02, Steam Fable.exe)

| Metric | Value |
|---|---|
| Functions recompiled | **57,102** (every catalogued entry + discovered call targets) |
| Instructions lifted | 3,267,132; **0 unsupported, 0 undecodable** |
| Indirect jumps left to runtime dispatch | 544 (vtable/import/function-pointer thunks; no unresolved switch tables) |
| Generated C | ~225 MB in 143 files; compiles and links into one binary (~10 min, 2 cores, gcc -O1) |
| Differential test (whole game, 5 random states each) | see `tests/DiffTest.cpp`; results below |

Differential test: every function runs in Unicorn (retail) and as recompiled C from byte-identical
memory (image, TEB, random pointer-dense heap, stack); EAX, EDX, ST0, stack, heap and all of
`.data` must match. Functions whose retail run leaves the pure-CPU boundary (imports, unmapped
memory with random inputs) are skipped, not counted. Known benign mismatches: `rdtsc`/`cpuid`
readers (host-dependent by design).

## Not done yet (the road to a running game)

1. **Win32 / CRT layer**: implementations of the ~400 imported functions (msvcr71, kernel32,
   user32, winmm, advapi32, gdi32, imm32, ole32, wsock32) on top of SDL3 / POSIX.
2. **Direct3D 9 layer**: `IDirect3DDevice9` subset + shader-model 1.x–2.0 bytecode translation
   (from `shaders.big`) to Vulkan/GLES; `d3dx9_25` helpers.
3. **DirectInput 8, DirectShow (WMV video), audio** → SDL3 input/audio, FFmpeg video.
4. **C++ exceptions / SEH** (`_CxxThrowException` + frame handlers) and threads.
5. Android packaging (ARM64; guest memory reservation; storage access to the user's install).

## Build

```sh
# Zydis + Zycore (from source), then:
cmake -S rebuild/recomp -B build/recomp -DCMAKE_PREFIX_PATH=<zydis-prefix>
cmake --build build/recomp
build/recomp/fable_recomp Fable.exe rebuild/manifest/functions.tsv gen/        # whole game
build/recomp/fable_recomp Fable.exe rebuild/manifest/functions.tsv gen/ --only a88b60,a52450

# generated code must be built without FP contraction:
gcc -O1 -ffp-contract=off -fno-strict-aliasing -Irebuild/recomp/runtime -Igen -c gen/recomp_0000.c
```

Differential test (needs the Unicorn oracle from `../modern/oracle`):

```sh
g++ -std=c++23 -O1 -ffp-contract=off -Irebuild/modern/oracle/include -Irebuild/recomp/runtime \
    -c rebuild/recomp/tests/DiffTest.cpp
# link with RetailOracle.o, runtime.o, gen/*.o, -lunicorn
FABLE_EXE=Fable.exe ./difftest gen/recomp_functions.txt 5
```
