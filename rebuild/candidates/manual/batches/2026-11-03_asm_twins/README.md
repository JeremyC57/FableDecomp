# 2026-11-03 asm twins -- retail inline-asm helpers in shared two-path headers

Owner rule (2026-11-03, "clean C++ for a port; identical logic; asm only where retail itself ran it"): no `__asm` in a
row source; every retail inline-asm helper lives in one shared header with the byte-matching `__forceinline` asm path
(+ `// Retail-asm:` evidence) and a `#ifdef FABLE_PORTABLE` C++ twin; a dual-path test per helper; matching grades
unchanged. Nothing here is committed, labelled or synced: the coordinator lands it.

Files: `INVENTORY.tsv` (all 181 landed retail_asm rows + the 5 chunk-721 pending ones: helper, asm block, semantics,
state, grade), `STATUS.tsv`, `recheck_grades.tsv`, `verify.log` (final regrade), `portable_twins_results.tsv`,
`NAME_FIXES.tsv`; labels in `ghidra_out/labels_asm_twins_2026-11-03.tsv`.

## Helpers (one header each)
| header | retail name (evidence) | asm | semantics | twin |
|---|---|---|---|---|
| `engine/GFFloatToLongNear.h` | FableWin.pdb `?GFFloatToLongNear@@YIJM@Z`: debug body 19 B / 9 instr = exactly a debug frame around `fld ; fistp ; mov eax` (same arithmetic gives GFROR13 18/9 and Floor 58) | `fld v ; fistp r` | fistp by the control word's RC = round to nearest, ties to even (NOT floor/trunc); NaN / out of range -> 0x80000000 | B: `fable_x87_twins.h` X87FistpNear |
| `engine/GFFastFloatToLong.h` | `?GFFastFloatToLong@@YIJM@Z`: 29 B / 12 instr = the pointer-out form (`lea ; mov [p] ; fld ; mov edx,p ; fistp [edx]`) | `fld v ; mov edx,&r ; fistp dword [edx]` | as Near | B |
| `engine/GFFloatToLongFloor.h` | `?GFFloatToLongFloor@@YIJM@Z` (58 B = retail 0x0048D940) | 10 instr + jne | r = fistp(x - 0.5); `inc` when float(r + 1) bit-equals x. No RC switch. Quirk kept: 2^24 -> 2^24 + 1, -2^31 -> -2^31 + 1 | B |
| `engine/GFFloatToLongCeil.h` | `?GFFloatToLongCeil@@YIJM@Z` (58 B = retail 0x004FA9A0) | 10 instr + jne | r = fistp(x + 0.5); `dec` when float(r - 1) bit-equals x; -2^31 wraps to 0x7FFFFFFF | B (32-bit wrap explicit) |
| `engine/GFGetCycleCount.h` | `?GFGetCycleCount@@YI_JXZ` (signed __int64; debug 28 B / 13 instr = this block) | `xor eax ; xor edx ; rdtsc` | TSC; callers use differences scaled by the calibrated g_CyclesPerSecond | C: `__rdtsc()` (GCC/Clang/MSVC>=8), CLOCK_MONOTONIC off x86; VC7.1 has no intrinsic and keeps the block |
| `libpk/FtoL.h` | FableWin.pdb `?FtoL@@YAJM@Z` (libpk's own copy of the bare round; out of line at 0x00C6B240) | `fld ; fistp` | as Near | B |
| `strmbase/EnlargedUnsignedDivide.h` | DirectShow BaseClasses wxutil.h `__inline` (vendor source, verbatim) | `div` 64/32 + optional remainder | valid for HighPart < Divisor (vendor precondition; else #DE) | B: 64-bit `/` and `%` |
`rebuild/include/fable_x87_twins.h` holds the shared portable models (X87FistpNear, FloatBits); it is compiled only under
FABLE_PORTABLE / FABLE_DUAL_PATH_TEST. Rounding note in every header: the twins model RC = nearest (the engine never
changes RC) and >= 53-bit precision control (CRT default 0x027F); under Direct3D's single-precision control word the
`x +/- 0.5` of Floor/Ceil is rounded to 24 bits first, which differs only for |x| < 2^-24.

Every header carries `// Retail-asm-helper: <Name>` (purity marker, below). The out-of-line copies (0x0048D940 Floor,
0x004FA9A0 Ceil, 0x00CB0160 EnlargedUnsignedDivide) are the header's helper emitted by taking its address
(`g_keep...`, as landed 0x005BC3ED already does), guarded `#ifndef FABLE_PORTABLE`.

## Rows (186 inventoried)
- **87 REWRITTEN, no asm left in the row**: 85 landed (38 GFFloatToLongNear, 5 GFFastFloatToLong, 26 GFFloatToLongCeil
  incl. the out-of-line 0x004FA9A0, 7 GFFloatToLongFloor incl. 0x0048D940, 1 Ceil+Floor 0x006A68F0, 2 GFGetCycleCount,
  4 libpk FtoL, 2 EnlargedUnsignedDivide) + pending hand721
  0x00B45FB0 / 0x00B46660 (Floor). Row-local helper definitions replaced by the `#include`, calls respelled
  (ROUND / FloatToLong / GFFloatToLong / FtoL -> GFFloatToLongNear; pointer forms `F(&r, v)` -> `r = GFFastFloatToLong(v)`;
  CeilToLong / FloorToLong -> GFFloatToLong{Ceil,Floor}); comment mentions of the asm keyword reworded (purity keys on
  the raw substring); a `// Helper (2026-11-03 asm twins):` line added. 30 behaviour tests that carried a private asm
  copy now include the header (28 by script + the two out-of-line rows' tests); 26 tests of rewritten rows still carry a
  private asm copy (shapes the script does not parse; harmless to the grade -- follow-up).
- **22 IN_ROW_TWO_PATH** (asm stays in the row under `#ifndef FABLE_PORTABLE`, C++ twin beside it): the asm is the row's
  own function or an inline member whose spelling differs per row, so there is no shared helper to move it into.
  - 12 SSE kernels with a retail non-SSE C++ fallback (C3DBoundingBox::CalcShortestDistanceToPointSquared in 0x00B49810
    0x00B4CED0 0x00B99BB0 0x00B9C2E0 0x00B9C500 0x00BF7DF0; NVectorMath::GFInterpolateVectors x3, GFClear4DVectors,
    CMatrix4x4::AlignedSetInterpolate, CParticleProgram::TransformVertices) + pending 0x00B46280: the `#ifndef` drops the
    SSE branch so the portable build runs retail's own fallback (Strategy B: retail ran it on non-SSE CPUs; equal to the
    SSE lanes up to float rounding order and the NaN / degenerate-box clamp). Why not a header: the six bounding-box rows
    spell the member's x87 fallback differently (tuned per context), so one shared definition would break rows; the
    NVectorMath kernels are out-of-line functions called from other TUs.
  - 0x00497890 GFROR13 (the row is the helper: out-of-line __fastcall, 315 callers) -- twin 32-bit rotate.
  - 0x009E1C70 GFGetFastSubFrameTimer: via GFGetCycleCount.h it graded 59v58 (retail subtracts the base in place on the
    asm-written local; the helper's result is a copy) -- asm kept, twin = GFGetCycleCount().
  - 0x00C89BE0 FtoL(double), 0x00CA3B10 vorbis_ftoi (row-own functions; twin X87FistpNear), 0x00CB0310 memmoveInternal
    (twin memmove), 0x00A5B850 GFInitVectorMath (portable: g_SSEAvailable stays false so every kernel takes its fallback),
    0x00A5C2B0 GFFinishPartialSlerp (per-lane twin), 0x00A44250 GFGetInterpolatedColour (16-bit-lane MMX twin),
    0x00A13C20 C3DCoordI::operator-= (the stray retail `maxps` on dead registers; portable drops it -- likely a
    post-build patch, no helper).
- **77 HELD, unchanged**: 70 rows in the fenced EAX/GameCODA range 0x00C5xxxx-0x00C7xxxx (decisions wave); 5 libpk 2^x
  rows 0x00C40E60 0x00C40EE0 0x00C42C30 0x00C42C90 0x00C42FB0 (the frndint/fscale/f2xm1 block is a per-TU static libpk
  function with no PDB name and 9 of its 14 copies are fenced: design its header + twin once in the libpk pass; note
  f2xm1 is a hardware approximation, a pow(2,f) twin agrees after the float store except at rare rounding boundaries);
  pending 0x00B44820 / 0x00B44A60 (whole-body SSE vertex-lighting loops with rcpps: Strategy C twin + test not written).
  0x00B44A60's Retail-asm line named no instruction (purity read it naked_asm); it now names movaps/mulps/rcpps.

## Grades (bytes unchanged)
Final regrade, `recheck.py` + `vland.py --recheck-landed`, 4 parallel splits: **106/106 landed rows touched = 14 MATCH +
92 RELOCATION_MATCH, every behaviour test PASS**. Pending hand721 (read-only, `FABLE_HDR_OVERLAY=pending/hdr`): 5/5
(2 MATCH, 3 RELOCATION_MATCH). No row needed a row-local fallback for the header helpers except 0x009E1C70.

## purity.py
Old purity read a row that calls a header helper as `genuine` (80 rows would have flipped). New: `classify(text,
include_dirs=None)` follows the row's `#include`s recursively (FABLE_HDR_OVERLAY, then rebuild/include); a header
declaring `// Retail-asm-helper: Name` is a helper header; a row whose code names a helper (call or `&Name`) is
`retail_asm` when the helper header passes the existing evidence rule AND the row keeps its own `// Retail-asm:` line
(retail address + one of the helper's instructions), else `naked_asm`; include-without-use stays `genuine`.
Dashboard-equivalent count over every build_candidates source: retail_asm 181 (unchanged), genuine / naked_asm /
emit_bake / toolchain_library unchanged; the only per-row differences against HEAD purity are the 80 rewritten rows.
Unit test: `tools/decomp_pipeline/tests/test_purity_helpers.py` (11 cases, OK). Limitation: a helper reached only through
another header's inline function (the row never spells it) is not seen.

## Portable build check
`python3 tools/decomp_pipeline/portable_twins_check.py --out results.tsv` (lane env sourced): **168 steps, 0 failed**.
- 7 helper headers compiled alone: VC7.1 asm path, VC7.1 /DFABLE_PORTABLE, g++ -std=c++03 -DFABLE_PORTABLE -pedantic.
- 7 dual-path tests (`rebuild/tests/portable/test_*.cpp`, 63 float inputs incl. +-0, halves both parities, 2^-25,
  denormals, 2^23/2^24 edges, +-2147483520, +-2^31, 3e9, 1e30, +-inf, NaN; golden values from the VC7.1 asm path under
  Wine, CW 0x027F): vc71-asm / vc71-dual (asm == twin == golden) / vc71-portable / g++ 64-bit portable all PASS.
  GFGetCycleCount: 1000 interleaved asm/twin reads monotonic. EnlargedUnsignedDivide: 14 precondition cases.
- 6 row drivers (`rebuild/tests/portable/rows/*_driver.cpp`, each #includes its in-row two-path row) built as-is and with
  /DFABLE_PORTABLE: outputs identical (GFROR13, FtoL(double), vorbis_ftoi, GFGetInterpolatedColour 200 cases,
  memmoveInternal overlap cases, GFFinishPartialSlerp on exact dyadic inputs).
- Every row source that spells FABLE_PORTABLE (the 106 landed rows touched) compiles under VC7.1 /DFABLE_PORTABLE.
- g++ -m32: SKIPPED (no 32-bit glibc headers in the container); the VC7.1 steps are the 32-bit build.
Not covered by a dual-path run: the 12 SSE-guard rows (portable = retail's own fallback, compile-checked only).

## Gotchas (one line each, for docs/pipeline/GOTCHAS.md)
- The bare `fld ; fistp dword` helper is GFFloatToLongNear (FableWin debug 19 B / 9 instr), not GFFastFloatToLong (29 / 12 = the `mov edx,&r ; fistp [edx]` pointer form).
- A `__forceinline` asm helper moved from the row into a shared header inlines to the same bytes (106/106); the one exception is asm writing straight into a local the C++ then updates in place (0x009E1C70: the helper's result is a copy, 59v58).
- An out-of-line copy of a header `__forceinline` / `__inline` asm helper is emitted by taking its address in the row (`g_keep... = &Helper;`, guarded `#ifndef FABLE_PORTABLE`).
- 0x004FA9A0 is GFFloatToLongCeil out of line (float arg, ret 4), not CTCBulletTime::GetBulletTimeTicksFromAbilityLevel (PDB: no argument = 0x0079C5F0).
- purity.py keys on the raw substrings `__asm` / `naked` / `_emit` even in comments: reword them in an asm-free row or it reads naked_asm.
- `#ifndef FABLE_PORTABLE` around `if (g_SSEAvailable) { asm } else` turns retail's own non-SSE path into the portable twin without touching the matching token stream.
- Wine cl: pass a header-check prelude inline, not through `/I<linux path>` (an unresolved include hangs cl until timeout).
