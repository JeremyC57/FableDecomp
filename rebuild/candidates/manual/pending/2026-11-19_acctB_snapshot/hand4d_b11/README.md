# hand4d batch 11 (2026-11-19) -- the second GameCODA row: HELD for the coordinator's decision (retail_asm vs library grade)
- 0x00C88E60 CReverbFnsSSE::ApplyFilterOrder2_quad(float*,const float*,float**,CFloatFilterParams**,unsigned int) -- RELOCATION_MATCH 681 +
  test PASS (strict read-only vland, purity `retail_asm`), first grade. No flags.
- Identity: byte-identical (relocations masked) to GameCODA 2.10 CODAApi.lib member ReverbFns_SSE.obj
  ?ApplyFilterOrder2_quad@CReverbFnsSSE@@QAEXPAMPBMPAPAMPAPAVCFloatFilterParams@@I@Z (681 bytes, @comp.id 0x0060/3077). lib_verify.py
  --source on a stub proves it with FABLE_LIBSIG_LIBDIRS=coda=$S/sdk/gamecoda/tree/CODA/lib/Win32: MATCH, 4/4 relocations proven
  (__real@3f800000 = 0x0122DED8), no repo change needed. The landed caller 0x00C75CE0 is byte-identical to the SDK's
  ?Process@CFloatShortReverb@@... whose REL32 +0xA0 targets this function (name fix below).
- Why asm: the SDK source of this function is C++ plus an inline `__asm` block. The CODAApiDbg.lib /Od body carries the same
  51-instruction SSE block as the release member (only pfData's frame slot differs: [ebp-0E4h] v [ebp-4]): fixed esi / edi / ecx / edx,
  `subps xmm0,xmm0` / `subps xmm5,xmm5` / `subps xmm6,xmm6` zeroings that a `movss` load overwrites, and no /Od spill of any __m128 value.
  Retail 0x00C88FEA `mov esi,[ebx+0Ch]` .. 0x00C89001 `subps xmm0,xmm0` .. 0x00C890AA `dec ecx ; jne` .. 0x00C890B5 `movaps [edx+80h],xmm2`.
- Shape (ASM_RULES 3.2): the row is plain C++ (the CodeView locals `__declspec(align(16)) float fData[40]`, `float* pfData`, `int i`; the
  /Od fill order a1, a2, 1-(a1+a2), b0, b1, b2, -(b0+b1+b2), h0, h1, gain; two `for (i = 0; i < 4; i++)` loops) around ONE macro call
  into the NEW shared helper header engine/ReverbFnsSSEFilterOrder2Quad.h (`// Retail-asm-helper: CODA_SSE_APPLY_FILTER_ORDER2_QUAD`,
  byte-matching `__asm` path + FABLE_PORTABLE twin fable_portable::ApplyFilterOrder2QuadSSE). No `__asm` in the row or its test.
- Attempt table (ASM_RULES rule 4) -- NOT the 30 C++ + 10 intrinsic forms: the /Od proof above shows the original was asm. Graded:
  1 intrinsic form (probe/00c88e60/intrin_a.cpp, an <xmmintrin.h> transcription of the kernel): DIFFER 893v681; VC7.1 hoists the ten
  coefficient rows into stack copies (movaps [esp+0B0h..120h]), picks its own registers, no `subps x,x`. Plain C++ cannot emit SSE at
  all under the base flags. COORDINATOR: decide whether the /Od proof waives the remaining table.
- Note: the release ReverbFns_SSE.obj also holds F32vec4 (fvec.h) COMDATs; they belong to the object's other functions -- this function's
  /Od body uses no F32vec4 (only the inline block and float stores).
- Portable twin: a plain transcription (per sample, lane l at l*128: w = (x + c2) + (a2*h1 + a1*h0); y = ((b0*w + (b2*h1 + b1*h0)) +
  c6) * g; h1 = h0; h0 = w; do-while on uiSamples; history rows written back). Dual-path test tests_portable/test_ReverbFnsSSEFilterOrder2Quad.cpp
  (for rebuild/tests/portable/): 1 / 9 samples and an in-place run on exact power-of-two data (asm, twin and reference bit-for-bit), plus
  one INEXACT case against golden bit patterns from an independent float32 computation (asm checked in every VC7.1 mode; the twin
  checked where float is single precision -- g++ x86-64 -- and skipped on VC7.1 x87, where the twin may differ in the last bit; the
  header states this FPU caveat). dualpath.log: vc71-asm PASS,
  vc71-dual PASS, vc71-portable PASS, gxx-portable PASS, gxx-m32-compile not runnable on this host (no 32-bit glibc headers, the
  official tool reports SKIPPED). Twin mutants: 11/11 killed (history shift, c6 row, gain, lane stride, write-back swap, a1/a2 swap,
  b1/b2 history swap, one sample short, c2 dropped, and the two re-associations x + (c2 + fb) / b0*w + (ff + c6) killed only by the
  inexact case); a commuted fb (IEEE-equivalent) survives, as it must. The row also compiles under VC7.1 /O2 /DFABLE_PORTABLE (no SSE in the object).
  Portability note: `__declspec(align(16))` on fData is MSVC spelling, as in the landed libpk rows; the repo has no shared align macro.
- Library-grade alternative (owner rule 2026-11-19), if the coordinator prefers it: alt_library/00c88e60.cpp (purity
  `toolchain_library`; alt_library/lib_verify.log MATCH, 4/4 relocations proven, run with FABLE_EXE + FABLE_LIBSIG_LIBDIRS) = body-less stub
  `// Toolchain-library: CODAApi.lib(.\Release\ReverbFns_SSE.obj) ?ApplyFilterOrder2_quad@CReverbFnsSSE@@QAEXPAMPBMPAPAMPAPAVCFloatFilterParams@@I@Z -- 681 bytes verified against Fable.exe 0x00C88E60`
  + this batch's twin + dual-path test. The retail_asm row is the stronger hand-back: it is
  reconstruction (the C++ halves are verified against retail) and needs no binary-only library.
- Overlay (hdr/, from today's repo): NEW engine/CReverbFnsSSE.h (class from the CodeView LF_CLASS 0x11C9: CReverbFns base, the seven
  virtual overrides, ApplyFilterOrder2_quad; size 4); NEW engine/ReverbFnsSSEFilterOrder2Quad.h (the helper); engine/CFloatFilterParams.h
  new opt-in FABLE_RETAIL_CREVERBFNSSSE_APPLYFILTERORDER2_QUAD = `friend class CReverbFnsSSE;` (Inferred: the /Od body reads m_fA[1..2] /
  m_fB / m_fGain directly, `mov eax,[ecx+8]` etc.; VC7.1 CodeView records no friends). THIS COPY OF CFloatFilterParams.h SUPERSEDES
  b10's (it also carries b10's block, unchanged); 0x00C77800 regraded RELOCATION_MATCH with it and vland re-run on both rows (vland.log).
- Test (row): body verbatim vs an independent double model, 1 and 7 samples, bit-for-bit; 21/21 mutants killed (both loops' bounds
  both sides and starts, every gathered field, c2 / c6 signs and terms, h0 / h1 fill and write-back, gain).
- NAME_FIXES.tsv: 0x00C88E60 ApplyFilterOrder2_quad (the SDK name wins over queue line c688 ?FilterGain4@CDspOps@@...) and the caller
  0x00C75CE0 ?Process@CFloatShortReverb@@QAEXPAUCFloatShortReverbParams@@PBVCFloatCyclicBuffer@@PAMPAPAMI@Z (landed as
  CReverbStage94::Process). Account A applies them.
- COORDINATOR DECISIONS: (1) route: retail_asm row (src/) or library grade (alt_library/); (2) whether the /Od-body proof waives
  ASM_RULES' 30 C++ + 10 intrinsic table (1 intrinsic form graded, 893v681); (3) the two Inferred friends (CHLRvbParams b10,
  CFloatFilterParams b11); (4) `__declspec(align(16))` on fData is MSVC-only spelling (no shared align macro exists; landed libpk rows
  do the same), so the row compiles for the portable build only under VC7.1 (/DFABLE_PORTABLE: OK); (5) the twin is bit-exact only
  in single-precision float evaluation (stated in the header).
