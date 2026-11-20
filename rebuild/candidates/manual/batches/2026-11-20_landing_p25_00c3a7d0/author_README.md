# 0x00C3A7D0 CRampedParam::ApplyGainToFrame (p25N, 2026-11-20)

**Grade:** RELOCATION_MATCH (273/273). No flag, no pragma, no inline device, no Inferred line. Shared headers only.

- **Strict vland:** passes (`vland.log`): LANDVERIFY_STRICT_HEADERS=1, FABLE_HDR_OVERLAY=`hdr/`, a fresh LANDVERIFY_WORK; behaviour
  PASS; no warnings. The row is in the catalogue and not landed.
- **Base:** my earlier local-model draft, DIFFER(272v273). Its residue was an x87 store order: retail stores the clamped gain
  before the 0 compare; the draft compared first. About 22 spellings and flag sets had been tried on it.
- **Library:** libpk audio. It has no PDB, RTTI or SDK source, so all names are descriptive (see NAME_FIXES).

## What made it match
1. **GetValue spelled as `m_bAngle ? WrapAngle(m_value) : m_value`, with WrapAngle a float-in/float-out helper.**
   - The wrap's arithmetic paths join with the value in st(0), so the clamp stores it (`fst`) and compares it popped (`fcomp 0.0f`).
   - **Witnesses for this text** (`evidence/getvalue_witnesses.txt`):
     - WrapAngle is the landed **0x00C0B4C0** (`float __stdcall`, `ret 4`). This inline text compiles to it: cmpcomdat SAME.
     - GetValue and GetTarget compile to the retail out-of-line copies **0x00C0B730** and **0x00C0B770**: SAME. Both wrap their
       member the same way, which is what a shared helper gives.
     - The landed TU-mates **0x00C3A8F0** IsSettledAtZero and **0x00C38510** stay RELOCATION_MATCH with this GetValue in place of
       their value-local spelling.
   - **The value-local spellings fail** (`evidence/hdr_getvalue_*`, the same row under an overlay with only GetValue changed):
     - early returns: DIFFER(288v273), giving `fstp ; fld`;
     - the assigning form: DIFFER(272v273), giving `fcom ; fst`, the old draft's residue.
2. **The frame duration as a product in a named local, then divided in the call:**
   `float ms = (float)frame.m_samples * 1000.0f; Update(ms / (float)sampleRate);`
   - Retail finishes `fmul 1000.0f` before it loads the rate for its unsigned fix-up.
   - One expression schedules the rate load first: DIFFER(273v273), 2 lines (`evidence/one_expression_timestep.cpp`).
   - Eight other one-expression and helper spellings give the same 2 lines.

## Header (`hdr/`, `hdr.diff`)
New opt-ins in libpk/CFader.h:
- **FABLE_RETAIL_CRAMPEDPARAM_GETVALUE:** the kPi / kTwoPi / kMinusPi constants (the landed rows' names), the inline
  `float __stdcall WrapAngle(float)`, and CRampedParam::GetValue / GetTarget.
- **FABLE_RETAIL_CRAMPEDPARAM_APPLYGAINTOFRAME:** declarations of `Update(float)`, the spelling the landed 0x00C25F40 / 0x00C45980
  bind, and of this method, plus a forward declaration of CCodaFrame.
- **The only `-` lines** are the three existing FABLE_STATIC_ASSERT, now pinned as FABLE_STATIC_ASSERT_AT(33 / 34 / 35), their
  original lines.
- **newflags.py:** no newly flagged rows.
- **Define ablation** (`evidence/define_ablation.txt`): each of the three defines gives SRC_FAIL without it.

## Test (`test/00c3a7d0.cpp`, the payload test; passes in vland)
- **Setup:** links the REAL object. Update and GetData are logging stand-ins; __ftol2 is the CRT's.
- **13 cases:**
  - no valid samples: nothing is called or changed;
  - gains 0.5 / 0.3 / 0.1 / exactly 0 / exactly 1;
  - clamping below 0 and above 1;
  - angle wraps both ways, and an angle value left unchanged;
  - a non-angle value above pi;
  - zero channels;
  - 441 samples at 44100 Hz giving exactly 10 ms.
- **What it checks:** Update's `this` and time step, and that it runs before GetData; every sample against the truncating
  unsigned-16 × gain / 32767 model; samples past channels × samples untouched; parameter and frame unchanged.
- **Mutants:** 12/12 KILLED, including two header mutants on WrapAngle and GetValue (`mutants/00c3a7d0.out`).

## Follow-ups for main (not done here)
- **Possible new rows.** 0x00C0B730 GetValue and 0x00C0B770 GetTarget are not in the catalogue as functions of their own. These
  header inlines are their texts (SAME).
- **Re-port option for landed TU-mates.** 0x00C3A8F0 / 0x00C38510 could be re-ported onto this header opt-in; they already
  byte-match with it.
- **Naming.** The landed 0x00C3A8F0 calls its class CRampedParam_00c3a8f0, while the shared header says CRampedParam. That is
  one class.
