# 00e76e90 (p25B, Account B, 2026-11-20): NScript::CV_ExposeMayorScript::HeroFlashesLamp(long) -- READY

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00E76E90 | CV_ExposeMayorScript::HeroFlashesLamp(long) | 1612 | RELOCATION_MATCH (no flags) | PASS (38 targeted + 20,000 random runs) | 30 / 31 killed (1 same code) |

- Strict vland (LANDVERIFY_STRICT_HEADERS=1, FABLE_HDR_OVERLAY=hdr/): `RELOCATION_MATCH PASS`, no warnings.
- Purity: plain C++, no asm, no flag.
- No header change.
- Inline control: line 2 `// Inline-budget:` says `#pragma inline_recursion(on)` inside `#ifdef _MSC_VER`. This is the guard the owner
  approved for the 7 landed inline_recursion rows (docs/journal/2026-11/2026-11-05_SESSION.md), and the inline-control decision of 2026-11-08 covers it.

## What the parked diagnosis missed
- The parked draft (chunks 839/844/859: 111 grades, best DIFFER 1613v1612) blamed a block-layout residue.
- The draft also used a hand-written `if (flashes == 0) {..} else { CTimer timer; ..}` shape with local names.
- The fix is the generated quest-script text built from Ego_d.pdb's locals and line table:
  - parameter `TimesLeft`;
  - function scope `_Fail_Val` and `LampTimer`;
  - one block {TargetPosition}.
- `LampTimer` at function scope means the zero-flashes arm ends in `return _Fail_Val.Return();` (line 185, 20 bytes), followed by
  `CTimer LampTimer;` (line 189).
- With that text and FABLE_FORCEINLINE CTimer::operator= / GetTime (the header contract of 99 / 87 landed rows), the row matched at the first grade.
- The if/else spelling also matches with the forceinline timer operators, so the block "residue" was the timer operators' inline split.
  The Ego_d scoping picks the early-return form.

## Ladder and per-device ablation (advisor decisions 2026-11-20)

| device removed | grade without it |
|---|---|
| `#pragma inline_recursion(on)` (the self call is then never expanded) | DIFFER(556v1612) |
| ... with /d2inlT200 or /d2inlS30 instead (flags do not expand a self call) | DIFFER(514v1612) |
| FABLE_FORCEINLINE on CTimer::operator= (plain inline) | DIFFER(1596v1612) |
| FABLE_FORCEINLINE on CTimer::GetTime (plain inline) | DIFFER(1589v1612) |
| both plain | DIFFER(1589v1612) |
| both plain + /d2inlT60 / /d2inlT90 | DIFFER(1525v1612) / DIFFER(1589v1612) |

- The CTimer ctor and dtor stay plain inline. Retail expands them at some sites and calls the COMDATs 0x00CD4450 / 0x00CD4470 at the others.
- No labelled out-of-line CTimer::operator= or GetTime exists anywhere in retail (ghidra_out/labels_*.tsv), which is consistent with forceinline.

## Source shape
- Ego_d.pdb line table (egod_lines/, lines 170..218) and the locals above.
- Generated text as the landed Mains: `TRetVoid _Fail_Val;`, `return _Fail_Val.Return();`, and each terminate check on its statement's line.
- `// Inferred:` lines:
  - (1) `LampTimer.GetTime()>0` (retail `test eax,eax ; jle` / `jg`);
  - (2) the literal 3 (`push 3`).

## Test (test/00e76e90.cpp)
- Links the real row object: the row, its one-level expanded recursion, the real recursive calls back into the row, and the CTimer ctor / dtor COMDATs.
- Every external is bound by /alternatename to a __fastcall stub.
- Game slots 7 / 70 / 72 / 185 go through a fake vtable.
- pDSTGame slots 87..90 are on a separate object, which proves the timer calls go through the global.
- Inputs are scripted: terminate, lamp-lit, timer-running (positive running value, 0 or negative stop value) and distance sequences, at depth 0..5.
- Each run is compared event by event against an independent recursive model. The events include every timer register / set(3) / get /
  deregister with its index, which proves each nested timer is deregistered LIFO on every exit.
- It also checks:
  - the stores (HeroFlashedLightsCorrectly / ResetLamp only);
  - the point lookup name and the distance arguments (hero, the point, MaxFlashDistFromStables);
  - the release of every string, thing and timer.

## Mutants (mutants/)
- 31 one-decision mutants, 30 killed.
- SAME_CODE (1): the point-name literal swap changes only a relocation target. The test's strcmp catches it.
