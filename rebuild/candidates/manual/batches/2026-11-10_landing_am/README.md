# 2026-11-10 landing am -- chunk 1012b (2 rows)

Source: `pending/2026-11-10_linux_hand1012b/`. vland --land RELOCATION_MATCH, tests PASS:
- 0x004A34F0 CWorld::AutoLoadRegionLeaving (at its true 564 bytes; BOUNDS_FIX 592 -> 564, 0x004A3730 AutoLoadInternal follows) and
  0x004A5E90 CWorld::AutoLoadRespawn. One lever for both: the inlined CWorld::LoadGameState written as one if / else if / else chain,
  so a missing "any directory" save falls out to the shared `return false` instead of its own return. That gives AutoLoadRespawn one
  shared path-destructor block (no `lea` hoisted above the `je`, 0x004A6117) and drops `path`'s frame weight 11 -> 10 in
  AutoLoadRegionLeaving, so the fullPath+heroState slot sorts first as in retail.
- Parked (differ/): EAUseProjectileWeapon 0x0062BEF0 (one frame-slot tie), EAControlledCreatureStrafe 0x0062BD80 (explicit has-weapon
  bool; the FableWin typed readers fixed the frame -- overlay opt-in CGAMEEVENTDATAPACKAGE_TYPEDREADERS in hdr/, not merged),
  GetHeroSummonedCreaturesList 0x008A8260 (retail returns `bl` from a separate dead-hero epilogue). Seven rows not started.
- BOUNDS_FIX: 0x0062B070 EAOpenHeroInformationScreen 1872 -> 891 (four unnamed CWorld EA handlers follow).
- tools/kr.py: an emulation of c2's final stack-slot sort (a K&R quicksort, c2.dll 0x107AE0C4) that predicts density-tie orders.
