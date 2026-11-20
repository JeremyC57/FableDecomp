# 0x00C77B30 CFloatReverbParameterTranslator::UpdateReflectionsDelay (p25N, 2026-11-20)

**Grade:** RELOCATION_MATCH (193/193). The asm grade is retail_asm, coming only through the shared libpk/FtoL.h helper; the row has no asm of its own. No flag, no pragma, no device, no Inferred line.
- **Strict vland:** passes (`vland.log`), with LANDVERIFY_STRICT_HEADERS=1, FABLE_HDR_OVERLAY=`hdr/` and a fresh LANDVERIFY_WORK. Behaviour PASS: the test links the row object.
- **Catalogue name:** the row was catalogued as `CReverbProcessor UpdateLineLengths`, a descriptive name over local models. See NAME_FIXES.tsv.

## Name (two sources)
`?UpdateReflectionsDelay@CFloatReverbParameterTranslator@@AAEXPAVCFloatReverbParameters@@PAVCHLRvbParams@@@Z`
1. **The SDK library member.** The body is byte-identical (193 B, relocations masked) to the release member of that name in GameCODA 2.10 CODAApi.lib FloatReverbParameterTranslator.obj. Its DIR32 at +0x6f names `_sc_DefaultReflectionData`.
2. **The landed caller.** The landed, byte-identical Translate 0x00C783F0 calls it at REL32 +0x11d. rebuild/oracles/reloc_bindings.tsv already binds 0x00C77B30 to this symbol.

## Text
The text is the CODAApiDbg.lib member's own body (debug offset 0x1800, 1376 B), plus its CodeView (`evidence/FloatReverbParameterTranslator_dbg_codeview.txt`, from llvm-readobj).
- **Locals:** uiReflectionsDelaySamples, iReflection, iInput, uiInputDelaySamples.
- **ds_ftoi** (`?ds_ftoi@@YAHM@Z`) is the SDK's ReverbMath.h header inline. The debug member carries an out-of-line copy: two range ASSERTs, `f -= 0.5f`, `fld f ; fistp i`, then +1 if negative.
- **ReflectionData** is from the CodeView LF_STRUCTURE (20 B): fOutputGain, fFeedbackGain, fMinFeedbackDelay, fDelayOneSecond, fInputDelay[1]. sc_DefaultReflectionData is a const ReflectionData[4] (80 B) with C linkage.
- The debug build's ASSERTs and OutputDebugPrintf traces compile away in release.

## Header changes (`hdr/`, `hdr.diff`: two NEW shared headers, add-only)
- `engine/ReflectionData.h`: the struct plus the extern "C" table (0x012BF0DC). Three layout asserts, pinned with FABLE_STATIC_ASSERT_AT.
- `engine/ReverbMath.h`: `inline int ds_ftoi(float)`. Its fld / fistp is libpk/FtoL.h's FtoL, which carries the asm and the FABLE_PORTABLE twin, so this header has no asm.
- newly_flagged_rows: none. No landed row defines ReflectionData or ds_ftoi.
- **Re-port debt (not done):** these landed CODA rows use file-local made-up spellings of ds_ftoi (RoundHalfDownToLong, FloatToLongTruncate): 0x00C78150, 0x00C77FC0, 0x00C76E90, 0x00C770B0, 0x00C6AEF0, 0x00C6A460. Landed 0x00C3FF70 `RoundHalfDownToLong` may be an out-of-line ds_ftoi; that is unverified.

## Ablation
| change | grade |
|---|---|
| final (inner `iInput` loop 1..2, `/ 0.05f`, plain `inline` ds_ftoi) | RELOCATION_MATCH |
| `* 20.0f` for `/ 0.05f` (`evidence/a1.cpp`) | RELOCATION_MATCH: the bytes do not decide it; the debug fdiv 0x3D4CCCCD does |
| no inner loop, fInputDelay[0] / m_auiInputDelay[1] written directly (`evidence/a2.cpp`) | DIFFER(190v193): retail's `mov eax,4 ... cmp eax,54h` induction becomes `xor eax,eax ... cmp eax,50h` |
| previous draft (local models, a row-local __asm helper, a single loop) | DIFFER(190v193) |

## Tests
- **`test/00c77b30.cpp`** links the row object. The private method is called through a `__fastcall` stand-in (`/alternatename`, the thiscall frame).
- **The table:** it defines sc_DefaultReflectionData with retail's fInputDelay values; the other fields are poisoned.
- **What it checks:** both inputs of all four reflections, and that nothing else in the parameter block is written. It covers five scenarios, one with negative products that exercises ds_ftoi's +1 step.
- **Mutants:** 12/12 KILLED (`mutants/00c77b30.out`), including both ds_ftoi decisions in the new header.
