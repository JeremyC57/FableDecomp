# hand4d batch 9 (2026-11-19) -- a RE-PORT of a landed row (coordinator lever 5 of 0x00774D20)
- 0x00774B40 CTCShop::GetFreeSaleItemPlaces(bool) -- RELOCATION_MATCH + test PASS (strict vland with --recheck-landed). No flags, no overlay.
  Replaces the landed local model rebuild/src/compiled/00/77/CTCShop_GetFreeSaleItemPlaces_00774b40.cpp (and its test) with FableWin.pdb's
  text over the shared headers (Ego_d locals). One Inferred line (the dead area's radius spelling). Test: 6 mutants killed.
