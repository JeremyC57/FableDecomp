# Hand chunk 179 (CWorldMap .. CZoomingComponent): 28 of 30 byte-exact

## What matched (28)
- CWorldMap (CWorldMap.cpp TU, local model: retail 12-byte STLport containers, so Maps +0x20, Granularity +0x50, Pitch +0x58, Area +0x5C, MapPlacement +0x6C, LoadedMaps +0x90; PDB header is 4..0x1C higher):
  - 004fcbd0 IsPosPassable(C2DCoordI): ConvertWorldMapCoordsToLocalCoords is defined in this TU, so it is an inline member here; returns the callee bool directly.
  - 004fcce0 IsPosCameraPassable(C2DCoordI) / 004fcc40 IsPosPassable(C3DVector) / 004fcd60 IsPosCameraPassable(C3DVector): `if (PeekMap(n).X(local)) return true; return false;` gives the re-test + `mov al,1`; 3D forms truncate X/Y with __ftol2 into a named C2DCoordI.
  - 004fcac0 ChangeMapPlacementValuesTo: plain nested loop over MapPlacement with bounds Area.BRY/Gran and Area.BRX/Gran written in the loop headers.
  - 00502460 AllocateNewMap: `long map = 0; for (unsigned i = 1; ...) if (!Maps[i].Used) { map = i; break; } if (map == 0) { map = size(); resize(size()+1); }` reproduces the jump-threaded loop exit and the `test esi,esi`.
  - 00506bb0 GetLevelDimensions: the heap CFileFormatMapHeader (0x10822, two 0x8400-byte palettes) owned by a scoped holder, so the result box is stored before `operator delete`.
- CWorldMap (theme/height TU 0x0063Axxx, conversions out of line):
  - 0063a300 / 0063a390 / 0063a5f0 / 0063a7e0 / 0063a660 / 0063a6e0 / 0063a760 / 0063ab10: when retail calls PeekMap AFTER the conversion and only then does `lea ; push &local`, write `C local = Convert(..); const CMap& m = PeekMap(n); return m.F(local);`. A coord/vector that shares the later local's frame slot is a temporary (`GetMapNumberAt(C2DCoordI(...))`, `IsPosInMap(C3DVector((float)x,(float)y,0))`).
  - 0063bb40: PeekMap called twice (unused reference + direct call object, argument pushed first).
  - 0063adf0 GetStrongestEngineThemeAt: direct `return PeekMap(n).F(block, strength)` (arguments pushed before PeekMap).
  - 0063b920 / 0063b890 / 0063b9b0 / 0063ba30 PointIsWithinCell*: `float blockX = (float)(long)p->X; return blockX == cell->X && cell->Y == (float)(long)p->Y && <fraction test>;` -- the truncated X must be on the LEFT of `==` to get retail's `fstp ; fld [edi] ; fld [slot]` store/reload (on the right VC keeps it with `fst`).
  - 0063bab0 PointIsWithinLineSegment: `(a<b?a:b) <= p && (a>b?a:b) >= p` for X then Y, `return a && b && c && d`.
- CTCWound::CWound: 007572d0 GetWoundStrength (unsigned Duration vs long durations, `(float)Duration / longField` gives fidiv); 00757340 GetScarStrength (the two divisor operands as named float locals give `fild ; fdivp` instead of `fidiv`).
- LSocket: 008f9780 ~LSocket (Close if Socket != -1, erase(find) on the global list, member buffers freed in reverse order).
- CTwoLongSets (placeholder name): 00ad4a70 empty dtor of a record with two STLport set<long>; inline clear() + header free.
- NUISystem::CZoomingComponent: 005469e0 ChangeState: both arms then ONE base call `CChangingStateComponent::ChangeState(state)` (VC duplicates the tail itself but keeps `push ebx`; separate per-arm base calls make the else arm push the constant 0).
- 00c1c1a0 StartTimer (unidentified __fastcall library helper): plain sequence of calls; returns the registration result.

## Parked for Fable 5.1 (2)
- 004fcb30 CWorldMap::GetMinPlayerDistanceFromMap DIFFER(128v127): retail loads only Maps.begin into edi before the two __ftol2 calls and forms `lea ecx,[edi+map*72]` after them, and the Set test is `lea esi,[ecx+eax] ; cmp byte [esi],0`; ours forms the whole element address before the calls and uses `mov esi ; add esi,eax` | tried: named block coord, temporary coord, non-const member, explicit C2DCoordI(const C3DVector&) ctor + named CMapInfo& | next: `const CMapInfo* maps = Maps.begin();` before the coord, then `maps[map].Area.GetDistanceTo(C2DCoordI(info.Pos))`.
- 004ff310 CWorldMap::GetAllLoadedMapsInArea DIFFER(124v123): registers match; retail homes the long->unsigned long push_back temporary and the empty __true_type tag both in the dead `maps` parameter slot with no frame, ours opens a 4-byte frame (`push ecx`) for the temporary | tried: named unsigned long local, `*it` twice (conversion temp), named long local, __forceinline by-value helper | next: lane gap: temporary in a dead argument slot (function has a named local).

## Skipped
- none.

## Label corrections
- 004fcc40 "IsPosCameraPassable" is CWorldMap::IsPosPassable(const C3DVector&) (callee CMap::IsPassableAt 0x0081F3D0).
- 0063a6e0 / 0063a760 "GetGroundSizeZAtBlock" are propagations: callees CMap::GetSoundAt (long result) and 0x0081F630 (bool result); named GetSoundAtBlock / IsPassableAtBlock here (inferred).
- 0063a7e0 "GetThemeMinCameraSizeZOffsetAt" calls 0x00820100 (CMap::GetThemeSizeZAt): probably CWorldMap::GetThemeSizeZAt; 0063a390 calls 0x008200B0 (labelled CGameScriptInterface::MsgIsActionModeButtonPressed, wrong).
- 0063b890 / 0063b920 / 0063b9b0 / 0063ba30 (all labelled TL/BR): by geometry BR / TL / BL / TR (PDB symbols ?PointIsWithinCell{TL,TR,BL,BR}@CWorldMap@@IBE_NPBVC3DVector@@0@Z all exist; confirm order).
- 00c1c1a0 "CWorldMap::SaveToFile" is an unidentified __fastcall library helper (real SaveToFile is 0x004FDA60, vtable slot 4); its 0x00C33xxx callees are mislabelled too.
- 008f9780 "CWound::Kill" is LSocket::~LSocket.
- 00ad4a70 "CWound::Kill" is the destructor of a two-set<long> stack record (ctor 0x00AD49C0) used by the primitive optimiser.
- 0x0051BE20 "CThingCreatureBase::GetAnimsWithNameStartingWith" is CArray<CMapInfo>::resize; 0x004CF810 "LTextTreeWalkThrough::BuildTreeArray" is an STLport _Rb_tree::_M_erase; 0x00A3A180 "_Dest_val<...CMusicManager...>" is a C2DBoxI distance helper; 0x0081F910 "CMap::CopyPos" is CMap::GetStrongestEngineThemeAt; 0x0050EE40 "CLayer::SetAllPolys" is the CFileLargeThemePalette ctor; 0x0051A240 is the CFileFormatLevel ReadFromFile.
