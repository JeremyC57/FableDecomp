# 0x00780430 CTCFlammable::GetClosestFirePointTo(const C3DVector&, bool) -- READY (p25Q, Account B, 2026-11-21)

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00780430 | CTCFlammable::GetClosestFirePointTo(const C3DVector&, bool) | 271 | RELOCATION_MATCH (no flags) | PASS (links the real row; ~30,000 scenes, 0..12 points, ties) | 19 / 21 KILLED (2 equivalent) |

Strict check: `LANDVERIFY_STRICT_HEADERS=1 FABLE_HDR_OVERLAY=hdr vland.py payload.json oracle.tsv` -> RELOCATION_MATCH, behaviour PASS, no warnings.

## Text
Shared headers only (CThing, NGraphicAppearance::CTCGraphicAppearance FOCAL, NFire_CFirePoint, CTCFlammable FIREPOINTS). Ego_d names and
types throughout (ptc_grap_app is `const CTCGraphicAppearance*`, TPI 0xB872; test_fire_point a `NFire::CFirePoint&`) and Ego_d's statement
list; the source comment decodes every Ego_d line size. What closed the old 273v271 draft (pending/2026-10-27_linux_hand510, an
appearance/closest register swap): line 378 (18 B) is `iterator it_best = it++;` (VC10 returns operator++(int) straight into it_best:
14 B + a 4-B EH state), which is retail's `mov ebp,esi ; add esi,2Ch` before the first GetHelperPoint; line 384 (113 B) is then a for
loop with no init statement. Line 394 (`it_best = it`, 12 B) before 395 (the distance store, 6 B) as Ego_d orders them.
No define beyond the class opt-ins: the default GetSquaredMagnitude gives retail's sum (the ZYX opt-in grades the same).

## Labels
- `// Inferred:` Ego_d lines 381 (482 B) / 389 (520 B) hold the two GetHelperPoint calls inside a debug-only check; release bytes are the
  plain calls (as the twin lines of landed SetAsBurning / SetAsSmoldering).
- No Fidelity copies, brackets, flags or inline controls.

## Header changes (hdr/, hdr.diff) -- strictly add-only
- engine/CTCFlammable.h: new opt-in FABLE_RETAIL_CTCFLAMMABLE_GETCLOSESTFIREPOINTTO declaring the private
  `std::vector<NFire::CFirePoint>::iterator GetClosestFirePointTo(const C3DVector& pos, bool unlit_only);` (+ a C3DVector forward
  declaration). No asserts moved (pin_asserts: 0).

## Mutants (mutants/): 19 / 21 KILLED; the two survivors are equivalent
- `return FirePoints.end()` -> `begin()` in the num_fp == 0 branch: the vector is empty, so begin() == end();
- `(pos - best_point)` -> `(best_point - pos)`: the squared length of a difference is symmetric.

## Names (NAME_FIXES.tsv)
UNMATCHED_ROWS.tsv lists the row as `_gapscan::sub_00780430`; it is CTCFlammable::GetClosestFirePointTo (Ego_r.pdb + Ego_d.pdb).

## Newly flagged rows / notes
None. The `iterator x = it++;` reading (VC10 ~18 B, no temporary) may also matter to the reserved 0x00763060 / 0x007602F0 decodes
(noted in their pending NOTES).
