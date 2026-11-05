# Asm cleanup under docs/pipeline/ASM_RULES.md (2026-11-05): 97 rows now reach asm only through shared helpers

## Result
- **97 of the 99 landed `retail_asm` rows had `__asm` in the row source.** They now `#include` a shared helper header. No row source
  or test contains `__asm`, `naked`, `_emit` or `volatile`.
  - **w3, 21 rows on existing helpers:** 20 libpk `fld` + `fistp dword` rows on `libpk/FtoL.h` (new `LIBPK_FTOL` macro), and
    0x009E1C70 on `engine/GFGetCycleCount.h` (`GF_GET_CYCLE_COUNT`).
  - **w4, 76 rows on 15 new helpers:**
    - `libpk/`: `MixKernelsSIMD.h` (36 rows), `Pow10.h` (15), `ReadTimeStampCounter.h`, `IsCpuidSupported.h`, `SaveAllRegisters.h`;
    - `engine/`: `C3DBoundingBoxSSE.h` (7), `NVectorMathSSE.h` (5), `CMatrix4x4InterpolateSSE.h`, `CParticleProgramSSE.h`,
      `GFGetInterpolatedColourMMX.h`, `GFROR13.h`, `GFInitVectorMathCPUID.h`, `C3DCoordIStrayMaxps.h`;
    - `libvorbis/vorbis_ftoi.h` and `strmbase/memmoveInternal.h`.
  - Each helper has a byte-matching asm path, a `FABLE_PORTABLE` plain-C++ twin, `// Retail-asm-helper:` + `// Retail-asm:`
    evidence and a dual-path test in `rebuild/tests/portable/`.
- **Coordinator regrade** (`logs/`):
  - gate.py: 97 / 97 PASS.
  - vland `--recheck-landed`: 29 MATCH + 68 RELOCATION_MATCH, behaviour PASS, with the same grade and parity as before.
  - purity: `retail_asm`, with no asm words in any row or test.
  - `portable_twins_check.py`: 386 steps, 0 failed. (The `-m32` syntax steps are skipped on this host: no 32-bit glibc headers.)
  - The dual-path test caught one twin difference, now fixed: Pow10's `frndint` keeps the sign of zero.
- **Rule 1 and 2 attempts** are in `w*/ATTEMPTS.tsv` and `ATTEMPTS_FAMILIES.tsv`: no plain C++ or intrinsic spelling reproduced any
  site.
  - VC7.1 has no `__rdtsc` or `__cpuid` intrinsics.
  - Its SSE intrinsics always realign the stack, which retail never does.

## Calls made by the coordinator (the owner may overrule)
- **0x00497890 GFROR13:** `_rotr` emits `ror` on a register only, while retail rotates the argument in memory. Together with the
  FableWin debug body's asm-helper shape, that is taken as original asm.
- **SIMD kernels:** the evidence is the missing stack realignment plus fixed register use. EXTRA_RULES 2026-11-03 already listed
  the SSE vertex kernels and libpk MMX mixers as retail inline-asm helpers.

## Not done
- **0x00B44820 / 0x00B44A60** (the rsqrtps / rcpps lighting kernels) keep their in-row asm for now. The owner decided
  (2026-11-05, ASM_RULES "Approximate instructions") that their twin is exact math checked within Intel's error bound. Their
  helper is the next job.
- **The 14 oracle bakes:** none matched (`DEFERRED.tsv`).
  - 0x005DACF0 and 0x007022C0 differ by compiler residue only, so they stay parked.
  - Two would call the existing GFFloatToLong helpers and were not attempted.
  - Eight are plain compiler code and were not attempted.
  - 0x009A8170 and 0x009A8840 are not catalog rows.
- **`purity_no_inline_asm.patch`** makes in-row `__asm` never count as `retail_asm`. It is held until the two kernels move.
- 0x00890750 was graded `naked_asm` only because a comment said "no naked". The comment is reworded and it grades `genuine`.
