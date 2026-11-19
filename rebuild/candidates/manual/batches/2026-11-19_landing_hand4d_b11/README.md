# hand4d batch 11 (2026-11-19, Account B): 0x00C88E60 CReverbFnsSSE::ApplyFilterOrder2_quad, landed as toolchain_library

**Advisor decision (owner delegated): library grade, not retail_asm.** The function is C++ wrapped around GameCODA's hand-written SSE block; the CODAApiDbg /Od body has the same block. The retail_asm route would need ASM_RULES rule 4's attempt table to be waived, and that table is not waived. The owner's 2026-11-19 GameCODA rule allows library grade for a row that still differs as plain C++, so this row takes it. The author's retail_asm version is kept unlanded in pending/2026-11-19_acctB_snapshot/hand4d_b11; choosing it would need an owner waiver.

## Proof
- **Byte identity:** byte-identical to GameCODA 2.10 CODAApi.lib member ReverbFns_SSE.obj `?ApplyFilterOrder2_quad@CReverbFnsSSE@@QAEXPAMPBMPAPAMPAPAVCFloatFilterParams@@I@Z`.
  - logs/lib_verify.log: MATCH, 681 bytes; all 4 relocations resolve to `__real@3f800000` = 0x0122DED8.
  - @comp.id 0x00600C05 (Utc1310_CPP build 3077) is a retail Rich-header entry.
- **Precondition:** the plain-C++ twin compiled as the row gives DIFFER(562v681) (logs/plain_cpp_twin_grade.txt, probe logs/plain_cpp_twin_probe.cpp). The author's intrinsic transcription gave DIFFER(893v681).
- **Identity, two sources:**
  - the member's own bytes;
  - the only direct call in retail, 0x00C75D7F inside 0x00C75CE0. That caller is byte-identical (432 bytes) to the SDK's CFloatShortReverb::Process, whose REL32 at +0xA0 names this symbol.
  - CODAApiDbg's CodeView names it as well.

## Files
- **Stub:** rebuild/src/compiled/00/c8/CReverbFnsSSE_ApplyFilterOrder2quad_00c88e60.cpp. It compiles to nothing in the matching build; under FABLE_PORTABLE it includes the twin.
- **Twin:** engine/ReverbFnsSSEFilterOrder2Quad.h, plain C++ only, with no __asm.
  - The audit fixed two things in it: the lane order (read all four lanes, then write) and the per-lane re-association of c6.
  - Its portability statement is in the header.
- **New header:** engine/CReverbFnsSSE.h, built from the CodeView class record.
- **CFloatFilterParams.h:** new opt-in FABLE_RETAIL_CREVERBFNSSSE_APPLYFILTERORDER2_QUAD adds `friend class CReverbFnsSSE`.
  - This is Inferred: the Dbg CodeView marks the coefficients private, and the /Od body reads them directly.
  - The twin needs it.
  - The default path is unchanged (hdrcheck2 default=same); its 3 landed users recheck clean.
- **Dual-path tests:**
  - rebuild/tests/portable/test_ReverbFnsSSEFilterOrder2Quad.cpp: exact, inexact-kernel and gather cases.
  - rows/00c88e60_ApplyFilterOrder2_quad_driver.cpp: exact data, 1002 lines identical.
  - The retail side links the CODAApi.lib member itself.
  - logs/portable_twins_with_sdk.tsv: every mode PASS.
  - logs/portable_twins_without_sdk.tsv: named SKIP for the retail side; the portable steps PASS.
  - The SKIP comes from the new `// Portable-check-requires-lib:` support in tools/decomp_pipeline/portable_twins_check.py.

## Portability statement
- **Exact (dyadic) data:** the twin is bit-identical to the member in every build, VC7.1 x87 included.
- **Inexact kernel data:** bit-identical where float evaluates in single precision with no contraction (measured on g++ x86-64 against float32 goldens). On x87 evaluation the last bit may differ.
- **Gather (c2, c6):** retail computes these on x87, so the result depends on the thread's precision control.
  - The twin equals retail under PC_24 (measured).
  - Under PC_53 the last bit can differ for inexact coefficients (measured).
  - Retail's actual precision control is NOT established. Direct3D 9 sets PC_24 unless FPU_PRESERVE is used. A bounded grep of the SDK tree for _controlfp is a follow-up.
- **64-bit host builds:** g++ x86-64 needs FABLE_RETAIL_PASS23_HOST_TEST_ABI. That is the project-wide rebuild_abi.h convention (1,768 headers include it), not a gap specific to this row.

## Names (for Account A)
NAME_FIXES.tsv:
- 0x00C88E60 takes the SDK name over queue line c688's CDspOps::FilterGain4.
- The caller 0x00C75CE0 becomes ?Process@CFloatShortReverb@@...
- 0x00C6A180 becomes ?ProcessShortReverb@CFloatReverbDspEngine@@AAEXPAPAMI@Z (landed as CReverbProcessor::ProcessStage94).

None of these is among the round-8 rows. B does not touch the landed callers.

Audit notes: audit_NOTES.md and audit_fixed_README.md.
