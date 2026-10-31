# Hand chunk 637 (CDriveManager .. CTCStealth): 9 of 10 byte-exact

Chunk 637 is the 10-row parked-retry chunk (CHUNK_BRIEF overrides the 40-row template). All nine matches are RELOCATION_MATCH,
all nine tests PASS under g++ (tall.sh) and the lane's VC7.1 (tall.py).

## What matched (9)
- CDriveManager::CreateDirectoryW 0x009E4DA0 (/GS): checks and selects the path's drive, walks the path items with
  _wchdir/_wmkdir, restores the drive. Lever: the PDB private inline `long ConvertDriveLetterToNumber(wchar_t)` (`l - L'@'`)
  gives the imm8 `add edi,-40h` and fixes the this/letter esi/edi swap. Converting before `_getdrive()` sets the order.
  Declaring `EDriveReturn result = 0` before the fullPath local hoists `xor ebx,ebx` above the first call. Return type
  EDriveReturn per the PDB.
- CNavigatorAStar::CalcWayPointWidths 0x00A1DE30: measures the navigable widths of the first two route legs into the
  steering agent's best-distance slots, then sets ProximityForArrival. Lever: the Ego_d CSteeringAgent inline accessors
  (GetRadius(bool) const, GetBestDistLeft/Right/NextDistLeft/NextDistRight float&, GetCurrentMaxSpeed, SetProximityForArrival).
  Also CalculateNavigableWidthOfLine respelled to its PDB reference parameters (moves `pop edi`).
- CTCHeroMorph::CreateAlphaChannel 0x0071BCD0: clears alpha where texture RGB equals mask RGB. Lever: C2DExtentsI with a user
  default and copy ctor lets both Lock temporaries pack over the GetDimensions temporaries (frame 0x20). The size test and
  the texel count go through the Ego_d inlines CTexture::GetWidth/GetHeight(level), which fixes the operand order.
- NParticleEngine::CParticleEmitterList::Insert 0x00AEB060: priority insert that erases dead emitters on the way. Levers:
  size() as STLport `distance(begin(), end())`, whose input-iterator __distance copies `__first` (`cmp esi,edi ; mov eax,esi`).
  The insert position is `--it; Active.insert(it, emitter)`. Parameter is the PDB's non-const CCountedPointer&.
- CLandscapeBackgroundPatch::CLandscapeBackgroundPatch 0x00BE5E70 (15-arg ctor): copies the placement, LOD and texture
  parameters and clears the buffers and bridge arrays. Lever: VC7.1 swaps consecutive parameter-copy stores in pairs, so the
  body order was derived backwards from retail's store order. Parameters as the PDB: longs truncated into ushort/uchar
  members, const lock-tree pointers.
- CSliderAnimator::Update 0x0056EB48: advances the slider animation and writes the value text and fill zoom. Lever: `fabs`
  from <math.h> (the C++ float overload) puts the fstp after the by-value argument slot setup. FindChild, SetZoom,
  CText::SetValue and GFIntToWideString are respelled to the PDB.
- CThingManager::LoadLoadableGlobalThingsFromFile 0x005223F0: when GlobalThingLoadingBehaviour == 1, loads the file's things
  for the world map's loadable sections and activates them. Levers: a named `CWorldMap* worldMap = WorldMap;` before a nested
  block holding the things vector. The vector packs into the dead insert-pair slot and its zero stores sink below `push esp`.
  Also CLevelLoader::GetLoadAllSectionName as a static __fastcall inline.
- CPerlinNoise::Initialise 0x00F35ED0: shuffles the identity permutation with the engine LCG and fills 256 random gradient
  vectors. Lever: GFFloatRandom(2.0f, &seed) - 1.0f inlined with agent635's named `double result = fmod(...)` (the next LCG
  imul/add is hoisted above the fmul). The shuffle keeps the written-out LCG step.
- CAIStateGroup_HoverHornetQueen::WaitWhileBabiesFight 0x0095C0B0: keeps out of the way while the queen is still runnable,
  the boss has living babies and is not ready for a phase change. Lever: the loop condition is the landed IsStillRunnable
  0x0095A420 body as an `inline` member. Its NAMED `CircleMarkers[0]` reference with two separate early-return guards gives the
  marker esi and the loop-carried boss edi. The previous best spelled the guards in the loop with IS_THING_ALIVE(marker),
  which rotates the registers. The in-loop lookup is the inlined CountBabies: an in-place STLport __lower_bound whose miss
  path keeps the previous boss value. Also dropped the old attempt's `#pragma pack`.

## Parked for Fable 5.1 (1)
- 0x007471F0 CTCStealth::DrawPreparePrimitivesForRendering DIFFER(308v308): `sub esp,40h` vs `44h` -- ours homes the ToWideString argument temporary in the dead `info` argument slot (`lea edx,[esp+60h]`), retail gives it its own frame word (`lea edx,[esp+24h]`), so all later esp offsets are 4 off and `mov ebx,[eax]` (engine vtable) sits after the Z store instead of before the pushes | tried: PDB signature (const CEnginePrimitivePrepareInfo&, renamed callees), C3DVector argument temporary from a named GetPos reference (fixed the fld/fadd/stores schedule), static-member flag through GetThingManager() (wrong order, 307), if-block vs early return, colour declared in the inner block, `*PDebugFont` operator* (313); the sweep already tried named wide local, wide assigned into a pre-declared local, extra frame word, colour before states, sink local | next: a same-TU stand-in or an inline wrapper for ToWideString/CEnginePrimitiveText that changes how the temporary is classified for dead-slot homing; otherwise lane gap: dead-argument-slot homing of a by-value class temporary

## Skipped
None.

## Label corrections
- 0x005223F0 "LoadGlobalThingBatch" (catalog, global) is CThingManager::LoadLoadableGlobalThingsFromFile (Ego_r.pdb).
- 0x007471F0 "CTCStealth::vfunc_11" is CTCStealth::DrawPreparePrimitivesForRendering (vtable 0x012689EC slot 11 + Ego_r.pdb).
- 0x007AEEC0 manifest "CountEffects" / landed "CountLivingHelperCreatures" is CTCBoss::CountLivingHelpers (Ego_r.pdb QAEJXZ).
- 0x00AEC6F0 landed as list<CCountedPointer<CSpawnedFuncBase> >::insert is list<CCountedPointer<NParticleEngine::CParticleEmitter> >::insert.
- Further spellings: NAME_FIXES.tsv.
