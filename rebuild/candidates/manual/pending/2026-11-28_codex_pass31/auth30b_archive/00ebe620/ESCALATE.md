# 0x00E70460 CDF_BeEvil::Main and 0x00EBE620 CVO_Woman::Main -- ESCALATE (AUTH30, 2026-11-27; one file for the pair)

## Measured (our gates, Codex's drafts as returned, no overlay patches needed)
- Bytes: RELOCATION_MATCH 1665/1665 and 1482/1482.
- pick-any: all EQ (pk_all 18 / 20 EQ; pickany_vs_retail 15 / 17 EQ, 0 NEQ). vftables: every bound table agrees, slot counts equal.
- Three other blockers besides Codex's source-review hold (the named `const CPredicateBase& guard = CIsThingAlive(Me);`, which no
  Ego_d / FableWin local supports):
  1. Each draft carries whole bodies of OTHER retail functions that are already landed rows:
     - COF_MainGates::Main and its ctor;
     - CWB_Guard1's ctor and Main (landed 0x00E16E00), as inline bodies;
     - strong Alloc definitions for CV_DemonDoorsScript / CV_OracleScript (landed 0x00E75590 / 0x00EBEE30), COF_MainGates (landed
       0x00CDEFB0) and CWB_Guard1 (landed 0x00E16D10);
     - strong GetParentScript / Init / OnPredicateFail of CWB_Guard1.
     That is the retail register_scripts.obj grouping rebuilt in row text: the owner decision of 2026-11-24 says compile-order residues
     wait for the object build mode and are not fixed with row devices.
  2. The strong Allocs are new LNK2005 duplicates of landed rows (pk_all reads them as EQ, so the checkers do not flag them).
  3. The named guard reference (gate 5, Ego_d) is Codex's own hold.
- No grade was spent on the guard spelling: with blockers 1 and 2 open, a spelling that drops the named local would still not land.

## Question
Do these two rows wait for the object build mode (with the scaffolding question of 00d05a60/ESCALATE.md for the entity tables)? Or does the
coordinator want a single-row form tried, without the other scripts' bodies, which would re-open the leaf?
