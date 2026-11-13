# 2026-11-11_acctB_c2029 -- acctB hand-back, chunk 2029 (2 reserved rows: 1 matched, 1 parked after pass 1)

Authored by an Opus 5.5 agent, re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x005FE090 CTCInventoryMap::ConstructQuestsScreen | RELOCATION_MATCH 1937v1937, test PASS (g++ and VC7.1) | plain C++, no flag, no device. Levers: the existing repo opt-in FABLE_RETAIL_CCOUNTEDPOINTER (c2 threads the null path into the else arm at 0x005FE23C); `description` declared before the `textBank` reference; locals constructed in the order of retail's destructor run. 2 grades |
| 0x00915B60 CAIStateGroup_MoveGoodsBase::PlaceGoods | parked, DIFFER 1923v1931 | 583 v 585 instructions, 533 of 585 equal with frame offsets and registers masked; control flow, cold pads, cross-jumps match. Left: a split cascade (exclusive_use ebp v retail esi at 0x00915F54 / 0x00915FFA / 0x00916014; crate_index ebx v ebp at 0x00916152; the vector start held in one ebx piece in retail 0x009161E9..0x00916254) and the scaled offset vector as one 12-byte node where retail has three scalars. 7 grades in pass 1; **passes 2 and 3 not used (agent context), and no behaviour test was written**. Source in `rows/parked/`. Re-queued in acctB |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT, no warnings). Purity and padded-struct greps are clean. Parked row
regraded: DIFFER(1923v1931), as recorded.

Review items for landing:

- NAME_FIXES.tsv (5): 0x00916640 is `CTCExclusiveUse::SetUser(CThing*)` (landed as `CClass_00916640::AssignTarget_00916640`);
  0x00915810 is the `CCreatureAction_PutGoodsDown(CThingCreatureBase&, const CCharString&)` constructor (landed with
  `(CThingCreatureBase*, long)`); the bound spellings of AIAction_MoveToPreCalcRoute 0x0097F690, FetchGoods 0x00915890 and
  GetFreeActionPoint 0x007FEDE0 differ from the PDB ones.
- The "" literal 0x0122D70E is bound in reloc_bindings.tsv to stand-in data names from other rows.

To land: the matched row needs only `hdr/engine/CTCInventoryMap.h` (c2025's copy plus the nested opt-in
`FABLE_RETAIL_CTCINVENTORYMAP_QUESTSSCREEN`): merge after c2025. The other overlay files (CTCExclusiveUse.h, CTCCrateStack.h,
CTCUserActionPointBase.h, CAIStateGroup_MoveGoodsBase.h, CCreatureAction_PutGoodsDown.h, the `_FILTER` opt-in in
CAIStateGroupBase.h, `FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D`) serve the parked row. Evidence in `hdr/README.md`. Rebuild the
oracle from Fable.exe (no retail bytes are in this folder).
