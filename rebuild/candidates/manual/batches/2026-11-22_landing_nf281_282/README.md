# Landing nf281_282 (Account A, 2026-11-22): the first two tracker batches (113 rows)

The first agents fed from the naming tracker (`naming/status/`, owner 2026-11-22: "batch items that are easier to work together").
Hand-backs: `pending/2026-11-22_nf281_handback/`, `pending/2026-11-22_nf282_handback/`. No header, flag or catalog change.

## NF281: T1-access + T2 (entries 1-53 of 234; 53 of its 55 rows land)
- 41 addresses done (33 T1, 8 T2), 2 rejected, 1 skipped (precheck), 8 census-only, 1 held. 55 rows changed, all zero line delta, in place in
  each row's own local model (access labels, `virtual`, `const`), as nf266 / nf268 did; objsec HEAD v new ALL_EQ under the renames for every
  row; vtmap of every table-emitting row unchanged; HEAD link sets leave the wrong spellings unresolved, the new sets resolve them, 0 new
  duplicates. 16 self-leaf renames (each defined by one landed source; catalog names unchanged), 49 caller respellings.
- **Held (rulings):** 0x0052C7E0 (D4: its change gives `?Update@CComponent@NUISystem@@UAEXM@Z` a second retail target, RELOC_CONFLICT, because
  0x005367F3's local model spells its call to 0x0052C7E0 as the base; held until 0x005367F3 is fixed) and 0x00A254A0 (D1: a `friend struct
  C3DMeshLODInfo_Methods;` naming a stand-in; held until ported onto engine/C3DMeshLODInfo.h, so entry 1 ends at 4 -> 2 spellings). 0x00A25E70's
  `friend class C3DMeshInfo;` names the real class as retail has it and lands.
- D2 (entry 24, a same-TU strong copy in 0x00951FB0 blocks the self respell) rejected as T3. D3: the clang precheck may add
  `-Wno-address-of-temporary`.
- Table: 12 stand-in lines deleted, 21 respelled (the 0x00531EC0 line, D4, kept as it was).

## NF282: T1 standin_alias (60 of 419)
- 60 `CCountedPointer<T>::DeleteData` addresses, each with one stand-in binder (a generated Reset or (T*) ctor row with a local `SCountBlock`
  model and a free `?Destroy_<addr>@@YIXPAX@Z`): the free declaration becomes the landed NF222 opt-in
  `FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY` + `engine/CCountedPointer.h`, the count block's initialiser `&CCountedPointer<T>::DeleteData`.
  objsec ALL_EQ under the single rename for all 60 (`.drectve` included); every new spelling the tracker's `pdb_spelling`; two sources each
  (FableWin name set + the retail binder body storing the target; T tied by the binder's own callers). Every target 2 -> 1 spellings.
  Link: HEAD leaves all 60 `Destroy_` references unresolved; the new set resolves 53 (the other 7 owners are still on stand-ins).
- **Rulings:** D1 the hybrid form (header-less local stand-in class pointing at the shared header's DeleteData) is accepted for binder-only
  respells; D2 the 66 binder / owner self names are queued as T3 ports; D3 the pinned `// Leaf:` lines stay (they make the binders' own
  stand-in spellings visible to the tracker).
- Table: 60 `?Destroy_<addr>` lines deleted, 51 PDB lines added.

## Recheck
`logs/recheck_*.log` (landed tree, 3 batches): 113 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT.
Rule slips (both agents: unchanged tests compiled inside rechecks without their own clang precheck; NF281 one `rm -f` in its work folder):
noted; the continuation briefs repeat the rules.
