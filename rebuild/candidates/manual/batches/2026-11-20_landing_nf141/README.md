# 2026-11-20 landing nf141: PRIORITY slices 7 and 9 leftovers (6 rows; 0x00994930 held)

Records: `naming/2026-11-20_agentNF141/` (selfcheck 0 problems). 4 entries: q4994 / q5140 done; q6182 done except 0x00BDC510,
0x00BE8920, 0x00BE7D70 (old no-Leaf landscape rows, left as found) and the self row 0x00994930 (held, below); q6408 open (NF144 has it).

## Rows (6 landed): RELOCATION_MATCH / MATCH, every test PASS (`logs/recheck.log`)
- q4994 / q5140: 0x00913F30, and 0x00964CA0 with its only caller 0x00964DA0, ported (the last pointer binders); 0x00964CA0 / 0x00964DA0
  also take leaf-only self renames (both PDBs). Self row 0x008FD000 re-authored as
  `?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ` (Ego_r 0x00847E50 + FableWin 0x0288D940; MATCH). All 55
  referencing rows bind it.
- q6182: 0x009FF590 ported onto engine/CGraphicDataBank.h (binds PBXK). 0x009FD6C0 (outside the entries) moves to the nf86 / nf88 pick-any
  scaffolding: its constructor is a header inline the ported 0x009FF590 now emits, which linked to LNK2005 against the old strong owner
  (NF141's proof `logs/nf141_*.log`). The two land together.
- 0x00964CA0 models the header-less CThingFilter_MarkerHasProperty as the landed 0x006A75B0 / 0x009642D0 / 0x00964EF0 do (no base);
  `(EMarkerProperty)1` carries an `// Inferred:` line (the enumerator is in no name source).

## Held: 0x00994930 (self rename PBEJ -> PBXK)
Renamed, its leaf `??0CMemoryDataInputStream@@QAE@PBXK@Z` collides with 0x00BF3B60 (an old no-Leaf landscape row, DIFFER), which defines
that constructor strongly from a local model: LNK2005 in the context link, absent at HEAD (the old leaf was PBEJ). Making 0x00BF3B60's
copy `inline` does not help (it still emits a COMDAT, and strong vs COMDAT is LNK2005), so the row stays at HEAD; queued. The 31
referencing rows already bind PBXK, as before.

## Headers (add-only opt-ins; token-identical without them apart from pin_asserts' `_AT` forms)
engine/CAIStateGroup_StealStealableItem.h `_ISRUNNABLE`, `_FINDPSTEALABLEITEM`, `_FINDPESCAPEPOINT`; engine/CGraphicDataBank.h
`FABLE_RETAIL_CGRAPHICDATABANK_CGRAPHICHEADER_LOAD`. No landed row defines any of them.

## Checks
- Split gate (GOTCHAS nf120): three universal targets dropped (above). 161 rows rechecked, 157 good; the 4 bad are the old landscape rows
  0x00BDC510, 0x00BE7D70, 0x00BE8920, 0x00BF3B60, each the same DIFFER with the HEAD headers. Gate 0; bindings_multi_check 1.
- Table: no set grew; 0x008FD000 3 -> 1, 0x009CBC20 3 -> 2, and 0x009FF590's binding of 0x00994930 moves to PBXK.
- One definition (`logs/symbol_diff.txt`): own leaves, shared-header inlines and COMDATs. The 17 landed owners of table-bound symbols the
  new objects emit were compiled and linked with them and every rechecked object (`logs/link_context.log`, 178 objects): no LNK2005
  involves a landing object, except the pre-existing `??0_List_iterator_base@std@@QAE@PAU_List_node_base@1@@Z`. 0x0041C780 defines it
  strongly, and the pick-any copies in 0x00913F30 / 0x00964CA0 clash with it, as landed 0x00964360 already does at HEAD
  (`logs/link_control_head.log`). Queued as DEBT (nf86 / nf88 on 0x0041C780). The other 18 LNK2005s are between untouched rows. Link
  test over the 6 objects: none.
