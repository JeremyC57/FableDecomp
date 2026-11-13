# acctB hand-back, near-miss retry 3011 (1 previously parked row: matched)

Authored by two successive Opus 5.5 agents (the second continued in the first one's directory) starting from c2029's
parked source, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00915B60 CAIStateGroup_MoveGoodsBase::PlaceGoods | RELOCATION_MATCH 1931v1931, test PASS (g++ and VC7.1) | **supersedes `2026-11-11_acctB_c2029/rows/parked/00915b60.cpp`** (was DIFFER 1923v1931). Plain C++, no flag, no device. 11 grades in this retry (g01-g07 first agent, g08-g11 second), on top of c2029's 7 |

What changed against the parked source (I diffed them; the function's exit handling only):

1. a separate `if (!goods.Get()) return false;` before the action-point GetTC (retail's hoisted `lea ecx,[esp+14h]`,
   0x00915BFC);
2. the PerformAction-failure `SetActionPointAsFree(index)` sits inside the pre-route `if`, and the bottom free falls into
   the function's final `return false` -- this moves c2's hoist of the by-value vector's `_M_start` load and gives
   retail's register assignment (crate_index ebp, exclusive_use esi, the vector start in one ebx piece);
3. the pre-route else is `else if (!goods.Get()) { return false; } else { action_points->SetActionPointAsFree(index); }`
   -- the explicit return copy adds one reference to `goods`, which breaks a density tie in c2's frame-slot sort and gives
   retail's slot order; the copy is cross-jumped away, so the instructions are unchanged.

All three are ordinary exit handling, no dead code. The measurements behind each step (c2 register-choice traces, a
replay of the frame-slot sort) are in `NOTES.md`; the mechanisms are in `LESSONS.md` for GOTCHAS.

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT). Purity and padded-struct greps are clean.

Review items for landing:

- vland RELOC_NAME on 0x007FEF40: the manifest labels it `CTCUserActionPointBase::FreeActionPoint`; both PDBs name it
  `?SetActionPointAsFree@CTCUserActionPointBase@@QAEXJ@Z` and reloc_bindings.tsv already binds that. The label is wrong.
- Descriptive names: the parameters `distance` and `route_thing` and all locals (no PDB parameter / local names here).
- NAME_FIXES.tsv / LABELS.tsv are c2029's (0x00916640 `CTCExclusiveUse::SetUser`, the PutGoodsDown ctor signature,
  AIAction_MoveToPreCalcRoute, FetchGoods, GetFreeActionPoint).

To land: `hdr/` is c2029's overlay unchanged (new CTCExclusiveUse.h, CTCCrateStack.h, CTCUserActionPointBase.h; opt-ins
in CAIStateGroupBase.h `_FILTER`, CAIStateGroup_MoveGoodsBase.h, CCreatureAction_PutGoodsDown.h,
`FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D`; CTCInventoryMap.h from c2025). Rebuild the oracle from Fable.exe (no retail bytes are
in this folder).
