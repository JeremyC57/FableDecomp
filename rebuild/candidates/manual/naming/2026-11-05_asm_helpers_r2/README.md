# Asm cleanup round 2 (2026-11-05): the lighting kernels move to a shared helper; no SIMD or rotate site matches as C++

Rules: docs/pipeline/ASM_RULES.md. Budget per site (owner): at least 30 plain-C++ spellings, then at least 10 intrinsic
spellings, every one recorded with what VC7.1 (RTM 13.10.3077) emitted. Scope: brief targets 1 and 3 (the oracle bakes are a
separate agent's).

## Result
| site | outcome | plain C++ | intrinsic |
|---|---|---|---|
| 0x00B44820 / 0x00B44A60 lighting kernels | new helper `engine/CEngineLightingSSE.h` (rows MATCH, `retail_asm`, no in-row asm) | 30 x 3 flag sets each | 16 / 12 spellings (40 / 24 compiles) |
| 0x00497890 GFROR13 | helper kept, evidence now proven | 32 | 32 |
| MixKernelsSIMD (21 macros, 36 rows) | kept, proven | 30 x 3 per family | 13 per macro |
| C3DBoundingBoxSSE (7 rows) | kept, proven | 30 x 3 | 10 |
| NVectorMathSSE (5 rows) | kept, proven | 60 x 3 | 51 |
| CMatrix4x4InterpolateSSE | kept, proven | 30 x 3 | 17 |
| CParticleProgramSSE | kept, proven | 30 x 3 | 13 |
| GFGetInterpolatedColourMMX | kept, proven | 30 x 3 | 10 |
| C3DCoordIStrayMaxps | kept, proven | 30 x 3 | 10 |

## Why nothing converts (ATTEMPTS_UNIVERSAL.tsv)
- **Plain C++:** VC7.1 has no vectoriser. 210 spellings over 7 operation families, under /O2, /Ox /G7 and /arch:SSE2, emit no
  packed SIMD instruction. The only xmm instructions are scalar `ss` ops and `movaps xmm,xmm` copies.
- **Intrinsics:**
  - VC7.1 gives every function that holds an `__m128` / `__m64` value and touches its stack a `push ebp ; mov ebp,esp ;
    and esp,-16` (-8) frame. The only exception is a body that lives entirely in registers, such as a `__fastcall` with
    every argument in ecx / edx and no spill.
  - VC7.1 folds only its own spill slots into SIMD operations. A load through a C pointer is always a separate `movaps` /
    `movdqa` / `movq`.
  - Every retail site has at least one of these shapes:
    - no realignment in a function that reads stack arguments: the kernels, the box, the colour, the clear / slerp, half of
      the libpk rows, the stray `maxps`;
    - a folded user load: `mulps xmm0,[edx+20h]`, `paddd xmm1,[edx]`, `cvtdq2ps xmm0,[eax]`, `punpcklbw mm0,[esp+8]`;
    - C variables stored to the frame and reloaded with no call between: the operands of an inline-asm block. Examples:
      0x00A5CEE0 `this`, 0x00AE0E60 source / target, and the GFInterpolateVectors double frame.
  - The kernels add `mov esi,0`, `or ebx,ebx` and `push` / `pop` inside the vertex loop, which VC7.1 never emits.
- **GFROR13:** VC7.1 never emits a memory-operand `ror`. Even `g = _lrotr(g, 13)` on a global is load / `ror reg` / store.
- **Closest misses:**
  - 0x00A5C300 c11: 92 v 89 bytes, differing only by the realignment.
  - 0x00A5CEE0 i08: 285 v 285 bytes, but 44 instruction lines differ (register choice, no `this` home).
  - 0x00AE0E60 p01: 445 v 457 bytes, missing the source / target homes.

## The lighting helper
- `engine/CEngineLightingSSE.h`:
  - `GF_SSE_CALC_SW_LIGHTING_NO_CLIP` and `GF_SSE_CALC_SW_LIGHTING_NO_CLIP_SEPARATE_MAIN_LIGHT`. Each is the retail block
    verbatim; labels are prefixed.
  - A `FABLE_PORTABLE` twin, `fable_portable::CalcSWLightingNoClipSSE`, that computes 1/sqrt and 1/x exactly (ASM_RULES,
    "Approximate instructions"). The tolerance, 1.5 x 2^-12 per approximate instruction, is stated next to it.
- `rebuild/tests/portable/test_CEngineLightingSSE.cpp`:
  - Cases with no approximate instruction on the path are bit-identical. These are: no lights, attenuation clamped to the
    ceiling, a light behind every vertex, the main-light stream and the scratch rows.
  - The rsqrt-only case is checked within 1.5 x 2^-12 and the rsqrt+rcp case within the composed bound.
  - Measured on this host's Xeon: at most 1.084 x 2^-12.
  - It passes vc71-asm, vc71-dual, vc71-portable and g++ -std=c++03 -pedantic.
- The rows keep their C++ (`count /= 4`, the `this` home) and call the macro. vland --recheck-landed gives MATCH / behaviour
  PASS for both, unchanged. Purity gives `retail_asm`, with and without `purity_no_inline_asm.patch`.
