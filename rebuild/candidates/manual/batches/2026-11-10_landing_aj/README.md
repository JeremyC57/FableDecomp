# 2026-11-10 landing aj -- chunk 1013 (6 CWorldMap rows, 2 landed CTCInventoryTrade rows re-authored)

Source: `pending/2026-11-10_linux_hand1013/`.

- vland --land RELOCATION_MATCH (logs/vland_land.log): CWorldMap::UpdateDestroyedThemeAreas 0x004FEC80, GetAllMapsInArea 0x004FF150
  (manifest name respelled from the raw mangled catalog name), LoadThingsWithSectionNames 0x00500EB0, UpdateNavMaps 0x00501990,
  LoadLevel 0x00502620, IsLineOfSight(const CThing&, const CThing&, bool) 0x0063E6F0. Levers: shared headers with the real PDB
  signatures (set<CCharString>, ELoadType), same-TU noinline callee bodies (C2DBoxI::Overlapping, AreaCoversOneLevel), the visible
  GetAllThingsInRoughArea body, the physics locals declared from-then-to. 0x0063E6F0's list destructor binds 0x0063F360.
- Re-authored (vland --recheck-landed RELOCATION_MATCH, logs/vland_recheck_trade.log): 0x0059CB90 AddWantedItemToList, landed as
  NInventory::CTCInventoryTrade on a local padded model, is CTCInventoryTrade (Ego_r / FableWin; renamed: files, ps1, catalog,
  manifest); 0x005A45F6 ConstructSellActionsList rewritten on shared headers. Both followed their landed PC / buy twins
  (commit dab326947). Three stale bindings of the old 0x005A45F6 source removed from reloc_bindings.tsv (the invented
  ??ACComponentMap operator[], CPlayer::GetControlledCreature, the namespace-less CDataBank::GetTextBySymbol); it now binds the real
  STLport map<CComponent*,CComponent*>::operator[] 0x005B3167. 0x0059EB0C / 0x0059EC48 still bind the invented CComponentMap
  ctor / dtor (queued, land1011ai).
- Headers (hdrcheck2 default=same): CWorldMap.h, C2DBoxI.h, CWorld.h NAVIGATORMANAGER, CNavigatorManager.h, CMap.h, CWADFile.h
  (retail class branch), CAFile.h, CTCInventoryTrade.h WANTEDLIST, CThingCreatureBase.h HEADPOS, CPlane.h LINE (hdr/README.md).
- BOUNDS_FIX: 0x0063AB90 GetThemeFaceNormalAt 608 -> 582.

Parked after 1 grade (differ/): HeightAreaChanged 0x004FEB70 (register choice), GetThemeFaceNormalAt 0x0063AB90 (x87 pops),
CreateLineSegment 0x0063B3F0 (a second _ftol), Find{Ground,Theme}CollisionPointWithinCell 0x0063D0A0 / 0x0063D4A0 (scheduling).
Not started: 0x00507650 CWorldMap::LoadWorldFromBinaryFile (recipe in NOTES.md).
