# Landing nf303 (Account A, 2026-11-23): T1 "other" made-up names, NF299's continuation (40 rows, 10 table lines)

Hand-back: `pending/2026-11-23_nf303_handback/`; helpers under `naming_batch/agent_tools/nf303/` (census303.py / sample303.py: the NF299 Q4
census and its open sample). All 802 entries plus the prefix item classified: 4 done, 7 done-partial (the 40 rows), 12 done / done-partial
conditional on Q2 (13 rows, NOT landed), 686 skipped-port (684 by census, 95 sampled), 5 ports, 2 identity research, 4 decision-Q1, 8 held,
77 open; no header, flag, catalog or test change.

- Landed respells (both PDB name sets + the donor call; objsec ALL_EQ under the rename, `.drectve` included): SetName -> SetCombatAnimation
  (8 combat-action ctors), DestroyObject -> DeleteData (0x007A1870's own local `CCountedPointer`), Init -> protected Initialise (3 rows;
  0x00A6C170 qualifies `CInitBaseClass::Initialise()`), PlayCutScene -> PlayCutscene, `protected:` on FinishAction / OnAnimationLoaded
  declarations (access only), ClearStrafingCombinationAnims private in the owner 0x008E9710 (Leaf full AAE) and its sole caller 0x008E9850,
  the CCombatActionBase(long) ctor protected (7 binders), CSmallVector<CCharString,8>::Clear protected (2 binders + owner 0x009D2110),
  `__stdcall AngleDifference` -> `__fastcall GFGetSignedAngleDifference` (11 binders; float-only arguments, identical bytes), Compare ->
  operator!= (0x006BBF20).
- Proof: links resolve each moved reference or leave it unresolved as HEAD did, 0 duplicates; every delete checked against every exe caller
  (call / jump sites, address references, a landed-source scan for virtual targets; the FinishAction / OnAnimationLoaded UAE lines stay).
  Staleness: all orig/ copies equal HEAD at landing (after nf300 / nf302).
- Table: the 10 unconditional lines (7 delete, 2 add, 1 replace). The 21 lines conditional on Q2 are NOT applied (`logs/table_conditional_held.tsv`).
- Rulings: Q2 the 13 rows whose PDB member-template declaration (GetSubDef<T>, GetPFirstThing<F,T>) adds a `/alternatename` line to
  `.drectve` stay T3 (standing ruling: a `.drectve` change is T3; 0x00894490's existing line is a precedent to review, not a licence), queued
  as 12 drectve units. Q1 `virtual` on a base that also flips the binder's own override from QAE to UAE (the PDB spelling) is allowed per
  row under NF299 Q3 + NF302 Q5 (no table / RTTI emitted, every landed binder of the old self spelling moves in the same landing), recorded
  in RENAMES section 1. Q3 the open entries that need parameter-type changes go to the T1-signature continuation. Q4 held entries are taken
  once their locks are free, after re-reading the landed rows.
- Queued: 703 T3 lines (684 census ports, 5 ports incl. sort_heap 734-736 and the four not-stale witnesses, 2 identity research 84 / 306,
  12 drectve units); NAME_FIXES_QUEUE: 10 self rows (0x007A4120, 0x00857E40, 0x008E9850 and the 7 entry-1 ctors).

## Recheck
`logs/recheck.log` (landed tree, the 40 rows): 40 RELOCATION_MATCH PASS, 0 DIFFER, 0 RELOC_CONFLICT.
