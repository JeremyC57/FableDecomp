# 2026-11-20 landing nf138: PRIORITY slice 7 sixth pass (9 rows)

Records: `naming/2026-11-20_agentNF138/` (selfcheck 0 problems). 3 entries: q4747 done; q4802 blocked with rows left (0x00762E10
CTCShapeManager family, 0x00B21DD0 needs a CCardinalSpline header claim, 0x00B21E90 inline-budget residue, 0x00764E90 / 0x00BCD850 /
0x00BCD8F0 / 0x00BCD950 STLport instances whose leaves the landed 0x00BCDB10 / 0x00BCDCA0 bind); q6408 open (10 of 34 referencing rows
bind the claim; 6 not reached). Both are unassigned until a slot frees.

## Rows (9): 3 MATCH, 6 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- q4747 family, landed whole (advisor rule: the add-on moves with 0x0064BE45): 0x00733480 onto engine/CTCCombat.h; 0x007334B0
  CThing::GetPBaseDef -> CTCCombat::GetPMeleeBlockCounterAbility; 0x00733290 fn_operator -> CTCCombat::IsMeleeBlockCounterValid const (both
  PDBs plus Ego_r function order; the old names are in neither PDB); binders 0x0064BE45 and 0x0068ECA0. Every address ends with one
  spelling (0x00733290 2 -> 1); no address in the family has a data or vtable reference, so none is virtual.
- q4802: 0x00B20B60 (STLport vector instead of the CKeyVector stand-in), 0x00A6CB20 onto engine/CPhysicsMesh.h (own parameter becomes the
  two-PDB `std::vector<C3DVector>&`) together with 0x00A12840 (own leaf now the spelling its referencer 0x0075B000 already binds).
- q6408: 0x00848FD0 ported.

## Headers (add-only declarations; default path token-identical)
engine/CTCCombat.h FABLE_RETAIL_CTCCOMBAT_GETPMELEEFLOURISHABILITY / _GETPMELEEBLOCKCOUNTERABILITY / _ISMELEEBLOCKCOUNTERVALID;
engine/CPhysicsMesh.h FABLE_RETAIL_CPHYSICSMESH_GETRIGIDBODYBOUNDINGPOINTS. No landed includer defines them.

## Checks
- 114 rows rechecked, 114 good; gate 0 created splits; bindings_multi_check 1.
- Table: no set grew. Shrank: 0x0075B8D0 5 -> 3, 0x0073ACC0 7 -> 6, 0x00733EE0 3 -> 2, 0x00733290 2 -> 1.
- One-definition (`logs/symbol_diff.txt`): own leaves plus CDefPointer / vector template COMDATs and shared-header inlines. Link test over
  the 9 objects: one LNK2005, `CVectorMap<ETCInterfaceType,CTCBase*,...>::LowerBound` in 0x0068ECA0 vs 0x0064BE45. It exists at HEAD with
  the same two objects (`logs/link_control_head.log`) and is queued as debt.
