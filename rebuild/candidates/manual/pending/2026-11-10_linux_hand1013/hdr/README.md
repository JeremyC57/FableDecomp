# chunk 1013 header overlay (FABLE_HDR_OVERLAY=$A/hdr)

Every file here is the repo's `rebuild/include/engine/<name>.h` with new opt-in blocks only (each under its own `#ifdef`;
`diff` against the repo header removes no line). Default paths are unchanged. Apply by inserting each block at the place noted.

| Header | Opt-in | What it adds | Used by |
|---|---|---|---|
| CWorldMap.h | `FABLE_RETAIL_CWORLDMAP_UPDATEDESTROYEDTHEMEAREAS` | protected `UpdateDestroyedThemeAreas()` (Ego_r `IAEXXZ`) | 0x004FEC80 |
| CWorldMap.h | `FABLE_RETAIL_CWORLDMAP_AREACOVERSONELEVEL_BODY` (+ `_AREACOVERSONELEVEL` declaration-only `#elif`) | under `_MSC_VER` a `__declspec(noinline)` in-class body of `AreaCoversOneLevel(const C2DBoxI&) const` (the landed 0x004FB160 four-corner `GetMapNumberAt` test): the same-TU visible body lets VC7.1 see the callee does not capture the box | 0x004FF150 |
| CWorldMap.h | `FABLE_RETAIL_CWORLDMAP_LOADTHINGSWITHSECTIONNAMES` | `LoadThingsWithSectionNames(const std::set<CCharString>&, ELoadType)` and static `__fastcall GetThingFilenameFromLevelFilename(const CWideString&)` 0x004FAFF0 | 0x00500EB0 |
| CWorldMap.h | `FABLE_RETAIL_CWORLDMAP_UPDATENAVMAPS` | `UpdateNavMaps(const std::set<CCharString>&, ELoadType)` | 0x00501990 |
| CWorldMap.h | `FABLE_RETAIL_CWORLDMAP_LOADLEVEL` | protected `LoadLevel(long, ELoadType)` (+ GetThingFilenameFromLevelFilename when the previous opt-in is off) | 0x00502620 |
| CWorldMap.h | `FABLE_RETAIL_CWORLDMAP_THEMEFACENORMAL` | `GetThemeFaceNormalAt(const C3DVector&) const`, `GetThemeSizeZAtBlock(const C2DCoordI&) const` 0x0063A7E0 | 0x0063AB90 (differ) |
| CWorldMap.h | `FABLE_RETAIL_CWORLDMAP_HEIGHTAREA` | `ClipAndConvertWorldMapAreaToLocalArea(long, const C2DBoxI&) const` 0x004FC4D0, protected `UpdateAllEdges()` 0x0063DFD0 | 0x004FEB70 (differ) |
| CWorldMap.h | `FABLE_RETAIL_CWORLDMAP_LINEOFSIGHT` | `ConvertWorldMapCoordsToLocalCoords(long, const C3DVector&) const` 0x004FC2A0, both `IsLineOfSight` overloads (C3DVector 0x0063AEE0, CThing 0x0063E6F0) | 0x0063E6F0 |
| CWorldMap.h | `FABLE_RETAIL_CWORLDMAP_LINESEGMENT` | protected `CreateLineSegment`, `FindGround/ThemeCollisionPointWithinCell`, `PointIsWithinLineSegment`, `PointIsWithinCellTR/BL/TL/BR`; public `GetThemeSizeZAt(const C3DVector&) const` 0x0063A390 | 0x0063B3F0, 0x0063D0A0, 0x0063D4A0 (differ) |
| C2DBoxI.h | `FABLE_RETAIL_C2DBOXI_OVERLAPPING_BODY` (+ `_OVERLAPPING` declaration-only `#elif`) | under `_MSC_VER` a `__declspec(noinline)` in-class `Overlapping(const C2DBoxI&) const` with the landed separating-axis body (`<stdlib.h>` abs) | 0x004FF150 |
| C2DBoxI.h | `FABLE_RETAIL_C2DBOXI_EXTEND` | corner constructor `C2DBoxI(const C2DCoordI&, const C2DCoordI&)` (FableWin, inline), `ExtendToContain` 0x00A39E20, `ExtendBordersBy` 0x00A39E10 | 0x004FEC80 |
| C2DBoxI.h | `FABLE_RETAIL_C2DBOXI_TOC2DBOXF` (needs `FABLE_RETAIL_C2DBOXF`) | `C2DBoxF ToC2DBoxF() const` (FableWin, inline: four `fild`) defined after the class | 0x004FEB70 (differ) |
| CWorld.h | `FABLE_RETAIL_CWORLD_NAVIGATORMANAGER` | inline `GetNavigatorManager()` / `PeekNavigatorManager() const` (FableWin 0x01C78D00 / 0x01C8E790), placed after `IsSerialisingHeroState` | 0x004FEB70 (differ) |
| CNavigatorManager.h | `FABLE_RETAIL_CNAVIGATORMANAGER_UPDATEAREA` | `UpdateNavigationMapArea(long, const C2DBoxF&)` 0x00A14E40 (inside the `FABLE_RETAIL_CNAVIGATORMANAGER` block) | 0x004FEB70 (differ) |
| CMap.h | `FABLE_RETAIL_CMAP_HEIGHTAREACHANGED` | `HeightAreaChanged(const C2DBoxI*)` 0x0081E810 (after IsPrefNavAt) | 0x004FEB70 (differ) |
| CWADFile.h | `FABLE_RETAIL_CWADFILE` | the retail polymorphic `class CWADFile : public CAFile` (Ego_r ctor `(CWADBank&, const CWideString&, FileAccessType, unsigned long)` 0x00A39D80, the CAFile overrides, private `Bank`/`Index`/`FilePos`, sizeof 0x10); the generated default class moves to `#else` | 0x00500EB0, 0x00501990, 0x00502620 |
| CAFile.h | `FABLE_RETAIL_CAFILE_MODIFIEDTIME_WACKY` | static `__fastcall GetFileModifiedTimeAsSomeWackyWin32BigNumber(const CWideString&)` 0x00999320 (before SPLITPATH_WIDE) | 0x00502620 |
| CTCInventoryTrade.h | `FABLE_RETAIL_CTCINVENTORYTRADE_WANTEDLIST` | public `AddWantedItemToList(long, NUISystem::CList*, NUISystem::CViewport*)` 0x0059CB90 (before BUYPOSSIBILITYPC) | rows/0059cb90 |
| CThingCreatureBase.h | `FABLE_RETAIL_CTHINGCREATUREBASE_HEADPOS` | `C3DVector GetHeadPos(bool) const` 0x00661B40 (after the STATTYPEFLAGS block) | 0x0063E6F0 |
| CPlane.h | `FABLE_RETAIL_CPLANE_LINE` | `Initialise(a,b,c)` 0x00A42170, `GetIntersectionWithLine(point, direction, out) const` 0x00A422C0, inline `const C3DVector& GetNormal() const` | 0x0063D0A0, 0x0063D4A0 (differ) |

Rebased: `CWorld.h` was re-copied from the current repo header (chunk 1012's SETKICKPOS / SAVEGAMESTATE / EVENTACTIONS blocks)
before the final gall run, so the overlay only adds the NAVIGATORMANAGER block.
