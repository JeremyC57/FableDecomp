# Asm cleanup round 2a (2026-11-05): lighting kernels onto a helper; every other helper proven; in-row `__asm` retired

The rules are in `docs/pipeline/ASM_RULES.md`, with the owner's raised attempt budget.

## Result
- **w1:** `engine/CEngineLightingSSE.h` holds the two hand-SSE lighting kernels, 0x00B44820 and 0x00B44A60
  (CEngineLightingManager::CalcSWLightingNoClip*).
  - **Asm path:** the retail blocks verbatim, as macros.
  - **`FABLE_PORTABLE` twin:** exact `1/sqrt` and `1/x`, under the owner's "Approximate instructions" decision.
  - **Dual-path test** (`rebuild/tests/portable/test_CEngineLightingSSE.cpp`):
    - outputs no approximate instruction reaches match bit for bit;
    - the rsqrt case is checked within 1.5 x 2^-12, and the rsqrt+rcp case within that bound twice;
    - the error measured on this host is 1.084 x 2^-12.
  - **Coordinator regrade:** gate 2/2, vland MATCH with behaviour PASS (unchanged), purity `retail_asm`.
- **w2:** comment-only "Proven" paragraphs in the eight round-1 helpers whose evidence had been judged: GFROR13, MixKernelsSIMD,
  NVectorMathSSE, CMatrix4x4InterpolateSSE, CParticleProgramSSE, C3DBoundingBoxSSE, GFGetInterpolatedColourMMX and
  C3DCoordIStrayMaxps.
  - **Coordinator regrade:** gate 53/53, vland 14 MATCH + 39 RELOCATION_MATCH (identical row for row), purity `retail_asm`.
  - `portable_twins_check.py`: 396 steps, 0 failed.
- **The attempts per site** (`w*/ATTEMPTS*.tsv`, `HANDBACK.md`) went beyond the owner's budget of 30+ C++ and 10+ intrinsic
  spellings: up to 60 C++ and 51 intrinsic spellings on NVectorMathSSE. Spellings were compiled under /O2, /Ox /G7 and /arch:SSE /
  SSE2. None converts, for these reasons:
  - VC7.1 has no vectoriser, so plain C++ never emits packed SIMD.
  - Every function that holds an SSE/MMX value and touches its stack is realigned (`and esp,-16/-8`).
  - A load from a C pointer is never folded into a SIMD operation.
  - Every retail site breaks at least one of those, for example a folded `mulps xmm0,[edx+20h]` or a stack argument read without
    realignment.
  - GFROR13: VC7.1 never emits a memory-operand `ror` (`_lrotr` on a global still loads, rotates in a register and stores).

## Purity
- `tools/decomp_pipeline/crawl/purity.py` now grades any row that spells `__asm` / `naked` as `naked_asm`. The evidence rule
  applies to shared helper headers only (`purity_no_inline_asm.patch` from round 1).
- **Coordinator scan of all 54,573 sources before and after the patch: 0 grade changes.**

  | Grade | Sources |
  |---|---|
  | genuine | 53,909 |
  | toolchain_library | 441 |
  | retail_asm (all through helpers) | 209 |
  | naked_asm | 8 |
  | emit_bake | 6 |

- No row source has `__asm` in code any more, apart from the 14 bakes (round 2b).

## Left for portability
- **24 helper headers keep an asm path,** each with a `FABLE_PORTABLE` twin.
- **The 14 oracle bakes** (round 2b).
- **441 `toolchain_library` stubs** (CRT objects and the vendor LZO `.asm`). A portable build needs C-library equivalents.
