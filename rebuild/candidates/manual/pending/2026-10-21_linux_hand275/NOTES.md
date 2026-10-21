# Hand chunk 275 (CWaveLoader .. CZoomingComponent): 14 of 40 byte-exact

(The chunk holds 20 rows; 14 match, 6 are parked as DIFFER.)

## What matched (14)
- **CWaveLoader** 00aad150 SaveWaveFile: builds the RIFF/WAVE file through two local CMemoryDataOutputStreams (format chunk, data chunk), then writes RIFF + size + both streams. Shape: channels read as `(short)`; CArray::GetData is `size() > 0 ? _M_start : (const T*)this`.
- **CWeaponDef** 004e8ce9 Transfer (leaf `?Transfer@CWeaponDef@@`, `#pragma optimize("s", on)`): 33 persist transfers. Shape: each defaulted CCharString member goes through an inline `Transfer(value, const CCharString& def)`, so the default's temporary is built in the dead [ebp+8] slot.
- **CWound** 00ad84d0 (really QuadricOptimiserInternals::COptimisedPrimitive::~COptimisedPrimitive, leaf `??1COptimisedPrimitive`): member arrays released with free(), an inline `_Destroy` loop per object array, a multimap `_M_erase` and clear.
- **CWound** 00c5ead0 (really CWound_00c5eca0::Kill, slot 5 of vtable 0x012B914C): kernel-streaming pin stop (state 2 then 0), stop event, wait for the thread, close handles, free three audio blocks. dllimport kernel32 calls.
- **CWorldMap** 0063ddf0 Validate() const: walks the loaded maps and converts the edge cells of neighbouring maps (the comparisons are compiled out). It matched first try; the `and esp,-8` frame comes for free from the block-local C2DCoordI temporaries in the loops.
- **CWorldMap** 0063e950 GetHeightZBelowAt(pos, radius, useTheme, filter): gathers things into a local STLport list<CThing*>, then runs the inlined CThingFindHeightZBelowAt body on each. Shape:
  - the ForEachInRoughArea instance is a same-TU auto_inline(off) stand-in, so the list sentinel stays in esi and its frame word is reused for `height`;
  - a C3DVector user copy ctor makes the circle copy direct, and copies from.X/Y through the FPU into `to`.
- **CZTargetNew** 00872490 InitialiseTransitionVariables: the x87 sums are accumulated in statement order. Other levers: `!(cross > 0.0f)`, a named `turn = acos*C`, and dividing by the long FramesForTransition each time.
- **CZTargetNew** 00872ec0 UpdateCoordSystem: flattened axis compared against epsilon squared; HasTC/GetTC forced-inline lookup; the CIntelligentPointer assignment builds a temporary, then `SetPItem(other.GetPItem())`.
- **CZTargetNew** 00873030 Update: reach the appearance through the inline `GetObservedThing()->GetGraphicAppearance()->...` chain; lerp the blend from a named float.
- **CZoomingComponent** 005467f0 ProcessEvent (this-adjusted secondary-base override, found by the Fable advisor):
  - the whole post-switch tail is duplicated into both case arms, each ending in `return;`, which moves `push edi` to the switch head;
  - C2DVector gets a user memberwise operator=;
  - position is built per member in a {} block.
- **CWoundMorphs** 00757990 TransferBinaryIn: reads the count, resizes the pair vector, reads the pairs as one block, re-sorts if dirty, then skips a trailing 4-byte field. Shape:
  - the trailing field is read back into `count`, so both reads use the dead stream-argument slot (frame 0x30);
  - CEntry has a user ctor that zeroes the longs and sets the float to 0.0f (the one immediate store; the rest share the ebx zero);
  - the inline Read tests `size <= m_remaining`, which gives `cmp eax,[ebp+0x14]; jg`.
- **CWorld** 0062aba0 EAUseQuickAccessItem (/GS; found by the Fable advisor from my 491v491 e3):
  - a same-TU auto_inline(off) noinline body for CInterfaceMap::LowerBound 0x0040F020 stops the 0x11 key from escaping, so it shares the dead `event` slot with the 0xA7 key, the ReadLong value and `handled`;
  - the third GetPlayer is staged through `long number = event->Player`;
  - the slot comes from a by-value forceinline ReadLong, so it stays in esi.
- **CWorldMap** 004ff570 ShowPassabilityOverlay: overlay enable per loaded map, then a width x height CRGBColour grid from IsPassableAt. Shape:
  - the blocked colour is declared before the passable one;
  - the box uses a four-long C2DBoxI ctor and is built before the colour vector (one `xor eax,eax` for both zeros);
  - GetData is `size() > 0 ? start : (T*)this`.
- **CWorldMap** 00500230 CreateNavMap, matched first try. It is modelled on the landed sources: GetLevelDimensions (CFileFormatLevel packed header), CMemoryFile::OpenFromFile (an inline CMemoryFile ctor/dtor around it) and the CNavigatorManager calls. The section finder 0x004FE630 returns {CCharString, long} by value.

## Parked for Fable 5.1 (6)
- 0063d960 CWorldMap::FindLastUnblockedGroundPositionAndReportCollision DIFFER(461v461): only the frame slots differ. Retail has direction +0x10, pos +0x1C, next/(to-from temp) +0x28; ours has next/temp +0x10, direction +0x1C, pos +0x28, so the length sum's load order follows. | tried: direction init vs assignment, C3DVector user copy ctor, decl-order permutations, next at function scope, named delta in a {} block, real pointer signatures + const | next: lane gap: frame slot ordering (VC appears to rank slots by reference weight; retail's next is ranked below direction/pos).
- 0063db30 CWorldMap::FindLastUnblocked(Theme)PositionAndReportCollision DIFFER(461v461): the same slot-order gap as 0063d960. It is the identical body with the theme callees 0x0063B7F0 / 0x0063D4A0. | tried: the 0063d960 forms | next: same as 0063d960.
- 0062bef0 CWorld::EAUseProjectileWeapon DIFFER(483v483): only the camera fetch differs. Retail puts the GetCameraManager out-local at [frame+0], shared with the spilled CTCHeroRangedAttacks pointer, and loads this/vtable/&camera into ecx/edx/eax. Ours shares it with the `this` spill at [frame+4] and uses edx/eax/edx. Also lea eax v ecx at the 0x48 key. | tried: forceinline HasTC + IS_THING_ALIVE through a CBaseObject* temp (fixed size), out-param GetTC, camera in a block, forceinline camera getter (named/unnamed), same-TU LowerBound bodies (either/both copies) + advisor 7 grades (camera at top/after ranged, out-param GetInterface, named source/sound, camera = 0) | next: lane gap: dead-slot packing order (advisor: park).
- 004fde80 CWorldMap::SaveGameState DIFFER(431v431): one frame slot. Retail stores the count copy at [esp+0x10] early (before the IsBinary test), with the traits temp in the dead context slot; ours swaps them and stores later. | tried: s1-s14 (copy forms, traits init, scopes), same-TU traits TransferBinaryIn stand-in, copy hoisted above the test, no copy (count stays in memory, +26 bytes) | next: lane gap: dead-arg slot packing order (advisor: the copy's address goes to the external TransferBinaryIn, so it cannot die early; park).
- 004feec0 CWorldMap::UnloadRegion DIFFER(444v444): callee-saved swap only. Retail keeps the region ref in ebx (`imul eax,0x58` before `push ebx`, then `lea ebx,[eax+ecx]`) and map/j in ebp; ours is reversed. | tried: u1-u7 (reference vs pointer, decl order, index scope) + advisor 3 grades (begin()+i, named base pointer, widened index) | next: lane gap: ebx/ebp callee-saved assignment (advisor: park).
- 004ff150 CWorldMap::GetAllMapsInArea DIFFER(437v436): retail keeps the `maps` reference in ebp for the whole body; ours reloads it from the arg slot. | tried: m1-m7 incl. same-TU Overlapping/AreaCoversOneLevel stand-ins + advisor 8 grades (pointer local/param, forceinline AddMap, named CMapInfo&, if-form __copy_trivial: 436v436 but maps in ebx) | next: lane gap: phi-coalescing / callee-saved tie (advisor: park).

## Skipped
None. Every row was attempted.

## Label corrections
- 0x00AD84D0 "CWound::Kill" is QuadricOptimiserInternals::COptimisedPrimitive::~COptimisedPrimitive.
- 0x00C5EAD0 "CWound::Kill" is CWound_00c5eca0::Kill, slot 5 of the RTTI-less vtable 0x012B914C (a kernel-streaming audio device; the landed dtor 0x00C5ECA0 uses that name).
- 0x0063DB30 "FindLastUnblockedGroundPositionAndReportCollision" is the theme twin, FindLastUnblockedThemePositionAndReportCollision (FableWin.pdb has both). Its callees are 0x0063B7F0 (theme max-Z in cell) and 0x0063D4A0 (labelled FindGroundCollisionPointWithinCell; really FindThemeCollisionPointWithinCell).
- 0x0063B750 "CWorldMap_FindMaxGroundZ" is `float FindMaxGroundZInCell(const C3DVector*) const`. 0x0063B3F0 CreateLineSegment and 0x0063D0A0 take pointers (`IBE_NPBVC3DVector@@0PAV2@1@Z`).
- 0x0063A390 (labelled GetGroundSizeZAt, duplicate) is GetThemeSizeZAt: the `useTheme` arm of GetHeightZBelowAt.
- 0x0063FD40's manifest functor type (CThingFindHeightZBelowAt) is wrong. That instance appends to a list<CThing*>, and its filter is {OverlappingCircle&, CThingFilterBase&}.
- 0x0084D050 "CPixelShader::CountReferences" is NCameraModes::CBase::GetWorld.
- The CWeaponDef::Transfer callees carry the junk label "UpdateShadowScene".
- 0x007597C0 "CSoundPairHeap_MakeHeap" is the CWoundMorphs map sort (CVectorMap<EBodyLocation, CEntry>, `__fastcall` first/last + comparator by value). 0x007595F0 "vector<CVectorMap<...>>::resize" is the CWoundMorphs pair-vector resize(n, const T&).
- 0x004FE630 "CreateNavMap_LoadParticleMorphs" is the CWorldMap navigation-section finder: it returns the chosen section's {name, offset} by value from a CMemoryFile and the wanted section set.
- 0x00789AB0 "C3DMeshInfo::HasPhysicsMesh" is a CTCHeroRangedAttacks const bool query (called after IncrementProjectileWeaponMode to choose player mode 0x19). Likely IsToUseFirstPersonTargetingMode; inferred, not verified.
- 0x0081E690 "GetWorldMapNumber" takes no arguments; it returns the 3D engine's handle for the map ([+0x20]->slot 13->slot 17(&WorldPos)). The extra pushes around it belong to the engine call.
