# chunk 1013 notes (CWorldMap rows + two CTCInventoryTrade re-authors)

All grades ran with `FABLE_HDR_OVERLAY=$A/hdr` (see hdr/README.md for every opt-in; nothing is written to the repo).

## Matched (src/ + test/)
- 0x004FEC80 `?UpdateDestroyedThemeAreas@CWorldMap@@IAEXXZ` RELOCATION_MATCH
- 0x004FF150 `?GetAllMapsInArea@CWorldMap@@UBEXABVC2DBoxI@@AAV?$vector@KV?$allocator@K@std@@@std@@@Z` RELOCATION_MATCH
- 0x00500EB0 `?LoadThingsWithSectionNames@CWorldMap@@QAEXABV?$set@VCCharString@@U?$less@VCCharString@@@std@@V?$allocator@VCCharString@@@3@@std@@W4ELoadType@1@@Z` RELOCATION_MATCH
- 0x00501990 UpdateNavMaps (same signature shape) RELOCATION_MATCH
- 0x00502620 `?LoadLevel@CWorldMap@@IAEXJW4ELoadType@1@@Z` RELOCATION_MATCH
- 0x0063E6F0 `?IsLineOfSight@CWorldMap@@QBE_NABVCThing@@0_N@Z` RELOCATION_MATCH. Relocation note: the out-of-line list
  destructor our object calls is `??1?$list@PBVCThingGameObject@@...` (STLP_LIST_DTOR_OUTOFLINE); retail 0x0063F360 is the folded
  COMDAT the naming pass proved as `std::_List_base<CThing const *>::~_List_base` (the catalog's `StdList_Clear_0063f360` /
  `CGuiControlTreePane::SortTreeRecursively` labels are the same address). The search call binds to the landed 0x0063FAE0.

## Re-authored (rows/)
- rows/0059cb90.cpp: **CTCInventoryTrade::AddWantedItemToList** `?AddWantedItemToList@CTCInventoryTrade@@QAEXJPAVCList@NUISystem@@PAVCViewport@3@@Z`
  (module CTCInventoryTrade, not NInventory), RELOCATION_MATCH on engine/CTCInventoryTrade.h (+ WANTEDLIST opt-in).
- rows/005a45f6.cpp: **CTCInventoryTrade::ConstructSellActionsList** `?ConstructSellActionsList@CTCInventoryTrade@@QAE_NPAVCComponent@NUISystem@@JJ_N@Z`
  (count is `long` per the PDB; the body copies it into an `unsigned long quantity`), RELOCATION_MATCH (+ FABLE_RETAIL_CTCSHOP_VALIDSTOCK).
- Tests rows/<addr>_test.cpp pass under g++ -std=c++03 and VC7.1 /Od.

## Parked (differ/)
- 0x004FEB70 HeightAreaChanged DIFFER(260v261): only the register choice. Retail keeps constant 0 in ebx (split around loop 1,
  rematerialised by `xor ebx,ebx` at 0x004FEC0B) and the second-loop iterator in ebp; ours: c2prio const 0 prio -12 vs iterator 54.
  CEngineManager::Get (0x00436FB0) is declared through a local `CTBaseSingleton<T>` / `CEngineManager` declaration in the source
  (no shared CEngineManager header exists: header debt). Next: a source construct that gives the zero more references in
  loop 2 or the iterator fewer; c2prio --trace on candidates.
- 0x0063AB90 GetThemeFaceNormalAt DIFFER(586v582), real extent 582 (SPLITS.tsv; the chunk catalog's 608 runs into 0x0063ADE0).
  Retail consumes fx/fy in each parity arm (`faddp`, `fcompp`); ours keeps them (`fadd st(1)` + `fstp st(0)`). 22 free probes
  (sum/compare/order permutations, hoisting) inert.
- 0x0063B3F0 CreateLineSegment DIFFER(836v856): retail calls _ftol on from->Y a second time for yHit.Y; ours CSEs it. Frame
  slots differ. Needs a spelling that kills the from->Y CSE (an aliasing store before it) without new code.
- 0x0063D0A0 / 0x0063D4A0 Find{Ground,Theme}CollisionPointWithinCell DIFFER(1017v1017): scheduling only (two spots). The
  corner-offset table 0x0137B730 is `extern C2DCoordI g_CellCornerOffsets[4]` (the landed 0x0063B750 name; no PDB name found).

## Not attempted
- 0x00507650 `?LoadWorldFromBinaryFile@CWorldMap@@QAEXAAVCWideString@@@Z` (identity proven, NAME_FIXES.tsv; `ret 4`). Needs:
  CDiskFileWin32 file(filename, READ, 2) in the frame; a CArray<char> buffer of length+1 (inline memset fill);
  file.Read(buffer, size, 0); CMemoryDataInputStream(GetAsCArray, size) 0x00994930; inline ReadSLONG; Maps.resize(n) out of
  line 0x0051BE20 then CMapInfo::LoadBinary 0x004FB4F0 for i = 1..n-1; Regions.resize(n) inline (CRegion() temp 0x006BC410,
  erase 0x0051CC80 / _M_fill_insert 0x0051D0A0, ~CRegion 0x0051A110) then CRegion::LoadBinary 0x006BC510; then per map
  SetMapPlacement(Maps[m].Area, m) 0x004FC9C0 and an INLINED CTBaseSingleton<CEngineManager>::Get() (instance 0x013BA854 /
  info 0x013BA858, CEngineManager vtable 0x01231584, deleter 0x004361E0) + CEngineManager::AddMap 0x006403A0. Header debt: a
  shared CTBaseSingleton.h (allocating Get) and CEngineManager.h (inline ctor with its list sentinel), CRegion ctor/dtor/LoadBinary
  and CMapInfo::LoadBinary opt-ins. The prior local model reached DIFFER(908v875).

## Identities / callees
See LABELS.tsv (rows and every callee named with Ego_r / FableWin evidence) and NAME_FIXES.tsv (CWADFile ctor 0x00A39D80,
LoadWorldFromBinaryFile 0x00507650, AddWantedItemToList scope, IsLineOfSight reference spelling).
