# Landing nf349 b1, b2, x1 (Account A, 2026-11-24): size-mode GetDef<T> copies in the 40 rows NF346 skipped

Retail compiled four GetDef<T> copies in size mode (98 B: frame pointer, `and [esi],0`):
- 0x0042AF3C GetDef<CThingBaseDef>, x31
- 0x0042B040 GetDef<CThingObjectDef>, x8
- 0x0042B312 GetDef<CUIMiscThingsDef>, x2
- 0x005F81BE GetDef<CExpressionDef>, x1

The REREAD define alone gave 108 B. Agent NF349's hand-back is in `pending/2026-11-24_nf349_handback/`.

## What changed
- **engine/CDefinitionManager.h:** four add-only nested opt-ins, `FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_{CTHINGBASEDEF,CTHINGOBJECTDEF,
  CUIMISCTHINGSDEF,CEXPRESSIONDEF}_SIZE_BODY`.
  - Each is a `template<> __declspec(noinline) inline` GetDef<T> carrying the GETDEF_GLOBAL_BODY body token for token, between
    `optimize("y", off)` / `("s", on)` and `optimize("", on)`.
  - Each is marked `Fidelity: scaffolding, not retail`, and raises `#error` without REREAD.
  - No existing line changes.
- **40 rows:** each gains `FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD` plus the opt-in for each of its types. The header landed
  once, with b1.

## Coordinator rulings
- **Q1, the form:** the per-type explicit specialization IS the 2026-11-20 size-pragma device: an inline definition wrapped in
  `optimize` to give a pick-any COMDAT the owner's body. The landed precedents are GETSUBDEF_CENTITYSOUNDDEF_SIZE_BODY and
  ENGINE_GETDEF_PASS27. Wrapping the template itself was measured on all 40 rows and changes nothing: VC7.1 takes an instance's
  optimize mode at the end of the TU (GOTCHAS).
- **Q2, x1 (0x005E5F30, 0x006096E0):** both rows join. They read RELOCATION_MATCH PASS before and after. Their leaf is data-equal and
  relocation-equal under stable section keys (tools/leafrel.py, with negative controls). objsec346's LEAF=1 is an artifact: dropping
  a COMDAT renumbers the sections it keys by.

## The three size-pragma conditions
- **(a) Grade and objsec:** the grade is unchanged, the leaf is identical, and only the target COMDATs change against the REREAD-only
  build.
- **(b) The owners set the same pragmas themselves:** 0x0042AF3C, 0x0042B040, 0x0042B312 and 0x005F81BE each set `optimize("y", off)` and
  `("s", on)`. None has a CompilerFlags entry, so all build at /O2 /Oy /W3 (2026-11-21 refinement); the opt-in comment cites them.
- **(c) Pragma order:** a scan of the preprocessed TUs finds no optimize pragma before the block.

## Gates
- **Both pick-any checkers, HEAD v new:** 0 turned NEQ and 0 newly NEQ, and no leaf-called NEQ GetDef copy is left. The reversed
  controls fail.
- **Dropped Assign helpers (22):** unreferenced across 731 objects.
- **Includer closure:**
  - equiv_check over the 1,210-row reverse closure: 0 differing rows. The plant flags exactly the 160 GLOBAL_BODY definers.
  - After landing, the coordinator rechecked all 272 compiled direct includers: 27 MATCH, 245 RELOCATION_MATCH, all PASS.
- **land_scripted recheck:** 40 of 40 landed, 0 failed.

## Debt / queue
- **Owner ports onto these opt-ins:** T3 nf349 0x0042AF3C / 0x0042B040 / 0x0042B312 / 0x005F81BE. The opt-ins themselves are scaffolding
  debt.
- **T3 updates:** the nf346 `port (size-mode copies)` line is closed (`T3_CLOSED.tsv`), and the nf340 lines are updated.
- **GetSubDef copies:** the 26 NEQ GetSubDef copies in these rows are unchanged and remain debt.
