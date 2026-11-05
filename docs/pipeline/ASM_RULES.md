# Asm rules (canonical; owner-confirmed 2026-11-05)

This page is the one statement of the asm rules. Where any other file in the repo disagrees with it, this page wins: CLAUDE.md,
EXTRA_RULES.md, the briefs, the handoffs, older journal entries. The earlier rounds are listed at the end.

## The rules
1. **Plain C++ first, always.** Try every C++ spelling before anything else: statement order, temporaries, casts, types, inline
   or out-of-line callees, and the flags the TU already uses. A row that matches as plain C++ is `genuine`.
2. **Go beyond plain C++ only where the original code used asm.** The owner, 2026-11-05: "the asm rule set past plain c++ rule
   should only be needed if the original code used asm."
   - **The evidence** is retail bytes that no VC7.1 C++ spelling emits, cited by retail address and instruction. Examples: a bare
     `fistp dword` (`(long)x` gives `__ftol2`, `/QIfist` gives `fistp qword`), `rdtsc`, `fnstcw`/`fldcw`, `cpuid`,
     `pushad`/`popad`, hand SIMD kernels.
   - **A compiler residue never qualifies:** register choice, scheduling, frame-slot layout, a block order or similar. Such a row
     stays parked until a C++ shape matches. Do not use asm, a helper or a pragma to cover it.
3. **Where rule 2 applies, take the first of these that matches:**
   1. **A compiler intrinsic** that emits the instruction (`_mm_max_ps`, `_rotr`, ...). It is C++, so the grade is `genuine`.
   2. **An isolated, macro-wrapped `__asm` in a shared header,** under the routine's PDB name. It needs:
      - a byte-matching `__asm` path;
      - a plain-C++ twin under `#ifdef FABLE_PORTABLE` with identical semantics (state the rounding mode or other FPU state the
        asm depends on);
      - a `// Retail-asm-helper: <name>` line and a `// Retail-asm: <retail address + instruction>` evidence block;
      - a dual-path test in `rebuild/tests/portable/`, run by `tools/decomp_pipeline/portable_twins_check.py`.

      Examples: `engine/GFFloatToLongNear.h`, `GFFloatToLongFloor.h` / `Ceil.h`, `GFFastFloatToLong.h`, `GFGetCycleCount.h`,
      `libpk/FtoL.h`, `strmbase/EnlargedUnsignedDivide.h`.
   3. **A `__forceinline` helper, only if necessary.** For example, VC7.1 never auto-inlines a function that contains asm, so a
      wrapper that retail expanded must be forced (`engine/GFPackFloat.h`).
4. **Each site keeps a table** of the plain-C++ and intrinsic attempts made before the helper, in the row comment or the batch
   README.
   - **Attempt budget** (owner, 2026-11-05): "absolutely confirm you can't figure out a way of using clean c++ first. i can clear
     you to increase attempts towards that goal." Portability is a main goal, and inline asm hinders it.
   - At least 30 distinct plain-C++ spellings per site, then at least 10 intrinsic spellings, before any helper.
   - Up to 60 C++ spellings where a family is a few bytes off.
   - Each attempt is recorded with what VC7.1 emitted.
5. **No `__asm`, `naked`, `_emit` or `volatile` in any row source or behaviour test.** A row reaches asm only by `#include`-ing a
   shared helper header.
   - Such a row is reconstruction: it lands, grades `retail_asm` and counts as genuine.
   - `naked` / `_emit` byte bakes (`naked_asm`, `emit_bake`, `asm_bake`) are oracles, never reconstruction, and never counted.
6. **Bytes the linker copied from a VC7.1 library or a vendor's shipped `.asm`** are a separate grade, `toolchain_library`
   (`docs/pipeline/LIBRARY_SIGNATURES.md`), and are not covered by this page.

## Approximate instructions (owner decision, 2026-11-05)
- Some instructions give results whose low bits vary between CPU families: `rsqrtps` / `rsqrtss` and `rcpps` / `rcpss`. Example:
  the hand SSE lighting kernels 0x00B44820 and 0x00B44A60. For these, the `FABLE_PORTABLE` twin computes the exact value
  (`1/sqrt(x)`, `1/x`).
- The dual-path test checks the asm path against the twin within Intel's documented bound: relative error at most 1.5 x 2^-12.
- The helper header states this tolerance next to the twin.
- This is the only case where a twin is not bit-identical. It applies only to instructions documented as approximate.

## Portability
- **Inside a helper:** the asm and its `__forceinline` sit only on the retail (non-`FABLE_PORTABLE`) branch. The portable branch
  is plain `inline` C++, and `portable_twins_check.py` builds it with g++ (64-bit run, plus a `-m32` syntax check).
- **Outside the helpers:** `__forceinline` also appears in Lionhead header helpers and row sources (EXTRA_RULES 2026-11-01), to
  match retail's inline expansion. This is NOT asm, and these rules do not touch it.
  - A non-MSVC compiler needs one compatibility define, `#define __forceinline inline __attribute__((always_inline))`.
    `engine/CSpawnedFunc.h` already does this locally.

## Existing code (2026-11-05 migration)
- **The landed `retail_asm` rows that carry `__asm` in the row source** (99 at 2026-11-05, allowed by the 2026-10-24 rule) move it
  into shared helpers, with identical bytes, or become plain C++ or intrinsics where those match.
- **The `naked_asm` / `emit_bake` oracle bakes in `rebuild/src`** (14) go through rules 1-3.
- **Behaviour tests that contain asm** (160) become plain C++ calls with the right calling convention.
- **`crawl/purity.py`** will reject `__asm` in row text once the migration lands, so that `retail_asm` is reachable only through
  a helper.

## History (each round superseded by the next)
- **2026-10-24:** `retail_asm` grade, with `__asm` allowed in the row plus a `// Retail-asm:` line.
- **2026-10-27:** evidence rule; no instruction-count cap.
- **2026-11-03:** asm only in shared helper headers, with a `FABLE_PORTABLE` twin (end goal: a portable port).
- **2026-11-04:**
  - priority order: no asm, then an isolated helper;
  - a per-site table of C++ attempts;
  - intrinsics and macro wrapping;
  - a fixed "impossible list".
- **2026-11-05 (this page):**
  - the fixed list is replaced by "the original code used asm", proven from retail bytes;
  - intrinsics, then the shared-header asm, then `__forceinline` only if necessary;
  - the in-row `__asm` allowance is retired.

## Status (2026-11-05, after asm cleanup round 2a)
- No row source contains `__asm` any more, apart from the 14 oracle bakes (round 2b).
- 209 `retail_asm` rows reach asm only through 24 shared helper headers, each with a `FABLE_PORTABLE` twin.
- `crawl/purity.py` now grades in-row `__asm` as `naked_asm`, never `retail_asm`.
