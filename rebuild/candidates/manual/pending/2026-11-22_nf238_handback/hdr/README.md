# NF238 header overlay (2026-11-22)

One header, one add-only opt-in.

## engine/NCreatureMode_CModeSitFloor.h
- Base: the repo working tree (`orig/engine/NCreatureMode_CModeSitFloor.h`, identical to the tree at hand-back time).
- New macro `FABLE_RETAIL_NCREATUREMODE_CMODESITBENCH_ALLOC`, inside the existing `FABLE_RETAIL_NCREATUREMODE_CMODESITBENCH_PASS27`
  class: `static CTCCreatureModeManager::CModeBase* __fastcall Alloc(CThing& owner);` (Ego_r.pdb / FableWin.pdb / Ego_d.pdb
  `?Alloc@CModeSitBench@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z`). Declaration only: no slot, no layout.
  Used by row 0x00742790 (and its test).
- The default path is token-identical. pin_asserts pinned 1: the `FABLE_STATIC_ASSERT(sizeof(CModeSitBench) == 0x38)` below the
  insertion is now `FABLE_STATIC_ASSERT_AT(85, ...)`, its original line.
- Includers (landed sources and tests): 21 rows include the header directly; no other header includes it
  (`grep -rl NCreatureMode_CModeSitFloor.h rebuild/include` is empty). None defines the new macro.
- Equivalence: `equiv_check.py` over two include-tree copies (repo tree v repo tree + this header), on the 21 includers' sources and
  tests: 0 differing (work/equiv.tsv empty; work/equiv.log). A first run before pin_asserts showed 0x00938380 differing (the
  assert's __LINE__); after pinning, none.
- Not touched by NF237 (its hdr/ holds only a README).
