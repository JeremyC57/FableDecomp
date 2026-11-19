# 2026-11-19 landing b791: 0x007791F0 is CTCDRegionExit::GetRegionName (Account B's request)

Account B's hand3c2 b2 (0x0064DD9A CDrawMiniMap::UpdateRegionExitNames) waits on this respell (COORDINATION_LOG; evidence in
`batches/2026-11-19_landing_hand3c2_b1_b7/audit_REPORT.md` b2): 0x007791F0 and 0x007792D0 are byte-identical; Ego_r / FableWin TU order puts
GetRegionName (index 4) at 0x007791F0 and GetNameOfRegionConnectingTo (6) at 0x007792D0. The exe scan finds no landed caller of 0x007791F0,
so the rename creates no split (0x00501450 keeps binding 0x007792D0).
- Row: self rename (file, Leaf, local model, catalog, manifest), in-place respell of its own local CTCDRegionExit model.
- engine/CTCDRegionExit.h: the comment now names both twins correctly and declares `GetRegionName() const` beside
  GetNameOfRegionConnectingTo (one assert line pinned with pin_asserts; assert_names_check 0 mismatches).
- Recheck: the row and every landed includer of CTCDRegionExit.h (10 rows): MATCH / RELOCATION_MATCH, every test PASS (`logs/recheck.log`).
  bindings_multi_check 1 (NullCoord).
