# 0x008D9B00 NCreatureMode::CModeScorpionKingVulnerable::AnimUpdate (p25R, 2026-11-21)

**Grade:** RELOCATION_MATCH (408/408). Strict vland (LANDVERIFY_STRICT_HEADERS=1, empty overlay, fresh work dir): WIN, behaviour PASS,
no warning (`vland.log`). **No header change**, no flag, no pragma, no Inferred line, no NAME_FIXES.

## What closed it (draft 8d_q12 406v408 on a local model; on shared headers v1 401v408)
- **Shared headers.** The define set comes from the landed TU-mate FrameUpdate 0x008E0A10. The body adds CScorpionKingBattleDef /
  CDefPointer with CDEFCLASSBASE_GETSUBDEF_BODY, CAnimSet / CAnimInit (LOOK_HEAD_ANIM, CANIMINIT_CTORS), CMainGameComponent,
  CMODESTANDBASE_ANIMUPDATE, the hit-location opt-ins and engine/GFLimit.h.
- **Ego_d statement order** (`egod_lines/008d9b00.odcmp_final.txt`). Lines 2256 frame_info (18), 2259 cur_frame (9),
  2260 clamp (25), 2261 last_frame (8), 2262 SetFrame (31).
  - Reading last_frame after the clamp gives retail's 0xC frame. Read first, the frame is 8 (residue v3 408v408, `8d_ab3`).
  - Line 2260 is a single statement: GFLimit<float,float,float>. Ego_d has that proc (`egod_lines/GFLimit_procs.txt`), and FableWin has
    ??$GFLimit@MMM@@YIXAAMABM1@Z. VC7.1 /Od gives 29 B with two literal temporaries; VC10 gives 25 (fldz / fld1 + fstp, 5 B each, against
    7 B for `mov imm`).
  - The if / else-if written in place compiles identically (`8d_ab1`). It is rejected because it would span several Ego_d lines.
- **IS_THING_ALIVE(PEyeEmitter)** on the intelligent pointer itself, as retail (three GetPItem calls). With `.Get()` VC7.1 keeps two:
  401v408 (`8d_ab2`). Its `!= 0` is the existing FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT operator.
- **Other Ego_d deltas** are the known VC10 ones:
  - 2250: +7, the def handle's EH state.
  - 2253: +6, the float cast spill.
  - 2262: +3 +3, two float-variable arguments.
  - 2270 / 2271: 55 v 38, the CCharString temporaries' EH states.
  - 2281 / 2283 / 2288: +3 each.
- **Lines with no release bytes:**
  - 2273 / 2274 (207 B each, after the two lookups) emit nothing in retail.
  - The GetSubDef / GetTC lines 2251 / 2268 (458 / 480 B, against 22 / 17 B here) carry debug-only code.
  - Neither is represented by text.
- **Names.** All are PDB spellings: GetFirstCombinationAnimFromType (0x0070C4E0) and GetPHitLocationWithDefName (0x00815820), as the landed
  rows are named. The labels DB still carries the old FindCombinationAnim / GetPHitLocationFromName labels (disassembler annotation
  only; vland raised no RELOC_NAME). The TU-mate FrameUpdate's Callees comment also says GetPHitLocationFromName.

## Ablation (one change at a time; sources in `evidence/`)
| change | grade |
|---|---|
| final | RELOCATION_MATCH |
| inline if / else-if clamp (`8d_ab1_inline_clamp.cpp`) | RELOCATION_MATCH (same bytes; rejected on Ego_d line 2260, a single 25-B statement) |
| `IS_THING_ALIVE(PEyeEmitter.Get())` (`8d_ab2_eye_get.cpp`) | DIFFER(401v408) |
| last_frame read before the clamp (`8d_ab3_last_frame_first.cpp`) | DIFFER(408v408): frame 8 v 0xC |
| without CDEFCLASSBASE_GETSUBDEF_BODY (`8d_ab4_no_getsubdef_body.cpp`) | DIFFER(422v408) |
| without CHITLOCATION_SETENABLED | SRC_FAIL (SetEnabled undeclared) |
| v1: first shared-header draft (`8d_v1.cpp`) | DIFFER(401v408) |

## Tests
- `test/008d9b00.cpp` links the REAL row object: `tools/rowtest.py`, and vland's fallback link.
- **Fixtures** (memory laid out at retail offsets): the mode, its creature (TC flags and TC map), the base def with a fake vtable
  (GetPSubDefInfo slots 14 / 15), and the battle def (delete slot 1, count +4, TailStrikePrepareTime +0x50). The anim set and an eye
  emitter (dead bit +0x91) are also built.
- **GetPDefFromGlobalIndex** is a fastcall twin under /alternatename (hidden return pointer, `ret 8`). The test therefore defines no
  CDefPointer whose inline COMDATs would compete with the row's copies. Under mut.py's link order (row object first), an earlier draft
  that did define one failed the handle-balance check.
- **Checks.** 8 cases against a double reference:
  - no anim set;
  - below, at and above the 0.8 threshold;
  - clamping at 0 and 1;
  - with and without TCs;
  - a live, dead or missing eye;
  - a last handle (the def is deleted).
- **Mutants:** 17/17 KILLED (`mutants/008d9b00.out`).
