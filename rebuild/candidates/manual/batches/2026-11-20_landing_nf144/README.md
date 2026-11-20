# 2026-11-20 landing nf144: mode-pointer and persist-traits leftovers (17 rows)

Records: `naming/2026-11-20_agentNF144/` (selfcheck 0 problems). 4 entries: q5199 / q5220 done (one unit); q6408 and q4802 blocked (NF147
has both remainders: the self row 0x0073ACC0 and 8 binders of q6408, the 7 rows of q4802).

## Rows (17): all RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- q5199 / q5220: 0x006D41F0 (NF136's last binder) calls CPersistTraitsSLONG::TransferIn through NF136's partial opt-in
  FABLE_RETAIL_CPERSISTTRAITS_SLONG_ONLY instead of the N006d41f0 stand-in. Only the 0x009E2590 binding changes.
- q6408 (16 binders): each row drops its local CModeBase / CCountedModePointer / CTCCreatureModeManager for engine/CTCCreatureModeManager.h
  (ADDMODEPOINTER) and re-bases its header-less local mode class on the shared nested CModeBase (the landed 0x0096DBC0 shape), declaring
  GetType() and resizing its pad so sizeof stays the retail allocation. Mode-ctor declarations are untouched, so their bindings are as at
  HEAD. Rows: 0x00848730, 0x008C1E80, 0x008C1F70, 0x008C7600, 0x008CB060, 0x008CB1A0, 0x008CCD70, 0x008ED950, 0x008F39A0, 0x008F41D0,
  0x008F4320, 0x008F4480, 0x008F47E0, 0x008F4B00, 0x00950C30, 0x00969CB0.

## Header
None changed (no opt-in).

## Decisions
- The 16 header-less local mode classes stay local, on the shared base (no header for any of them under rebuild/include; brief rule).
  0x008C1C30's CModeBriarRoseConcentrate has a header, so that row needs a full port (NF147).
- Mode-ctor PDB spellings (0x008E1150, 0x0084B7C0, 0x008E78C0) and 0x006D41F0's self rename are queued, not applied (no new spelling here).

## Checks
- 79 rows rechecked (gate referencing rows + renamed rows), 79 good. Gate 0 created splits; bindings_multi_check 1.
- Table: one set grew, 0x008CCC10 0 -> 1, a SAME witness (the address's own spelling). Shrank: 0x00718A00 5 -> 3, 0x0073ACC0 6 -> 4,
  0x009E2590 2 -> 1.
- One definition (`logs/symbol_diff.txt`): new rows emit the CCountedPointer<CTCCreatureModeManager::CModeBase> ctor / dtor / DeleteData
  as pick-any COMDATs; their owners 0x00718A00 / 0x007180B0 were compiled for the link. Landing link (17 objects): no LNK2005. Context link
  (rows + owners + includers, 97 objects): 5 LNK2005; the one that involves a landing object is GetEffectFromString between 0x008C1F70 and
  0x008C1C30, and the HEAD object of 0x008C1F70 clashes with 0x008C1C30 the same way (`logs/link_control_head.log`). Queued as debt.
