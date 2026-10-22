# Hand chunk 911 (CAIStateGroup_ApprenticeHero .. CPersistContext): 32 of 33 byte-exact

Boundary chunk: every row is a true function bound recovered from a mis-cut catalog row. All 32 matched rows grade
MATCH/RELOCATION_MATCH against `$S/batch/chunk911_oracle.tsv` and pass their behaviour test under g++ and VC7.1.
Note: `gall.sh` / the shared `$S/tools/q.py` grade 0x006BABD0, 0x007B5470 and 0x0092B110 as DIFFER (272v326,
180v371, 276v440) because older mis-cut oracles (chunk248 / chunk257 / bnd) shadow chunk911's row for the same
address; `q911.py` (chunk911 oracle first) grades all three RELOCATION_MATCH.

## What matched (32)
- **CPersistContext::Transfer<T>(name, value, default)** (0x00410BE0 long, 0x004AB710 double, 0x004CC3F0 short,
  0x006420F0 CRGBColour, 0x00685F80 COpinionTransientOffset, 0x006BABD0 CFaction, 0x007AD6D0 EScriptingStateGroups,
  0x00800B90 C2DVector): the speed-TU persist switch (`switch (Mode) {4,3,2,1,0; default: __assume(0)}` -> bare jump
  table). Shape knobs: where the CPersistTraits<T> object lives — named in the case-2 block (homes in the dead
  value-arg slot for 4-byte T), function scope (zeroed before the jump: EScriptingStateGroups; non-trivial
  ctor/dtor: CFaction), or none (inline traits: CRGBColour ReadColour/WriteColour, C2DVector ReadC2DVector which
  tail-merges, COpinionTransientOffset member TransferBinaryIn + virtual Write slot +0x10). double saves via
  `WriteLong((long)value)` (__ftol2). CFaction's text-load arm ends in `return` so `pop ebx` follows the traits dtor.
- **CPersistContext::Transfer<ESaveType>(name, value)** 0x004AB7F0: `Transfer(name, value, T())`.
- **CPersistContext::TransferBinary<signed char>** 0x004CC4D0: only cases 3 and 2, traits block local.
- **CPersistContext::TransferVector<T>** (0x00619890 ECategory, 0x007B5470 CCrimeWaitingForBribe: out-of-line
  `clear()`; 0x006D4C00 CNamedLong, 0x00F333B0 long: inline `clear() { erase(begin(), end()); }`).
- **CPersistContext::Transfer(name, vector<T>&) "Waves"** 0x00F27EA0 / 0x00F28610 (0x3C / 0x38-byte records): the
  landed 0x00F274D0 CCreatureEntry template (inline chunked ReadLong, out-of-line resize, null text-save entry).
- **CIVCountedPointer** operator= 0x006CA680 and copy ctor 0x0083CE60: polymorphic pointee (slot 1 Delete), release
  through an `old` local.
- **CDisplayManager** CalculateD3DPresentationInterval 0x009BE2D0 (one function, was SKIP in chunk 199) and
  GetRefreshRate 0x009BE340 (IDirect3D9 slot 8, `throw CCreateDeviceFailed()`, no /EHsc needed).
- **CGenericVar::ReadFromParser** 0x00A64360: switch 3,5,4,1,2 with the string reader inline.
- **CGraphicDataBank::GetTextureType** 0x009FD2B0: `long` transparency, `bool uncompressed = !IsCompressed()`, five
  result locals with type0 assigned first in every row.
- **CMemoryAllocatorBase::PublishAlignment** 0x00A643B0, **CMemoryAllocatorVariableSize::Set/GetPTreeRoot**
  0x00A64460/0x00A64470, **CMemoryAreaChunk::SetPermanent/SetDemandPaged** 0x00A64420/0x00A64440.
- **CLandscapeBackgroundPatch** InvalidateEdge 0x00BE5CA0 (direct 4-case switch), GetWorldMapX/Y 0x00BE5D40/0x00BE5D60
  (`Parent->Map->Map->WorldPosX + MapX`).
- **CAIStateGroup_ApprenticeHero::Update** 0x0092B110: retail tests the village-member bit twice; reproduced with a
  word-form `HasTC` guard and a byte-form `TestTC` inside the inline GetVillageMember accessor.
- **CNewFrontendGameComponent::Input** 0x0042E3EE (1441 bytes): size TU with frame pointer (`#pragma optimize("s",
  on)` + `("y", off)`) gives the compare-tree switches, `[ebp-4]` flag word and late ebx/esi pushes; the direction
  tests written `flags & 4 || flags & 0x40` merge to `test bl,44h` with the arms placed below the chain (a single
  `flags & 0x44` keeps them inline); KeyPressed takes `unsigned short` (the char arm pushes after `mov ecx,esi`).

## Parked for Fable 5.1 (1)
- 0x0064DD9A CDrawMiniMap::UpdateRegionExitNames DIFFER(521v580): retail `sub esp,4Ch` v ours `44h`; the empty CAllocatorTag/filter temporaries sit at `[ebp+0Bh]` (top byte of the live `region` arg slot) v ours `[ebp-1]`/`[ebp-2]`; RegionExits in ebx v edi; loop-2 node in memory with rotated entry | tried: frame-pointer size TU, tag with/without ctor, named allocator, list ctor default arg, separate loop scopes | next: lane gap: empty tag temporary overlaid on a live parameter slot (GOTCHAS)

## Skipped
- none

## Label corrections
- 0x006CA680 / 0x0083CE60 are CIVCountedPointer::operator= / copy ctor, not CCountedPointer.
- 0x006BABD0 is Transfer<CFaction> (chunk248 catalog: "Transfer<CIntelligentPointer<CThing>>", 326 bytes, mis-cut).
- 0x007B5470 is TransferVector<CCrimeWaitingForBribe> (another catalog: "Transfer<signed_char>", 371 bytes, mis-cut);
  0x00F333B0 / 0x00F27EA0 / 0x00F28610 / 0x004AB640 also carry the propagated "Transfer<signed_char>" label.
- 0x00618170 / 0x007B4BB0 "CComponent::SetParentColour" are vector<T>::clear().
- 0x006D44F0 "std::vector_insert_CLipSyncPhoneme" is vector<CNamedLong>::erase; 0x0046591D "VectorInsert_CopyRange" is
  vector<long>::erase.
- 0x00995030 "CPersistTraits<CFloatRange>::TransferBinaryIn" is CDataInputStream::ReadC2DVector.
- 0x004CBD50 labelled CPersistTraits<unsigned char>::TransferBinaryIn is the signed char instance.
- 0x00A64130 (landed as ReadFromParserAsSLONG) is ReadFromParserAsULONG (0x00A640D0 is SLONG).
- 0x006BC350 "MusicEntry_GetData" is CRegion::GetMiniMapRegionExitTextOffset.
- 0x008FCF40 "CXMVPlaybackInfo" is CAIStateGroupBase::Terminate; 0x007EE540 "IS_THING_ALIVE" is
  CTCVillageMember::GetPVillage.
- 0x00615540 "SortTreeRecursively" is the CThing* list dtor, 0x00615520 its ctor; 0x0065470C is the
  pair<C3DVector, CCharString> ctor; 0x00617CA0 / 0x00618E60 are CThingSearchTools::GetAllThings (list out-param).
- 0x0049BB50 "CTCInventoryBase::GetCanItemBeQuickAssigned" / 0x0049BB40 "DrawGetEnvironment" are the active-joystick
  singleton's (0x0049BB90) IsLocked (+8) / GetDeviceNumber (+4) getters as called from 0x0042E3EE.
- 0x009EA530 "CThingFilterIsUnconscious::operator" is called as CConsole::ProcessInput(const CInputEvent&).
- 0x00A03BB0 "DrawGetSeaLevel" is CInputEvent::GetMouseWheel (float +0x14); 0x00A03B30 "E2" is ~CInputEvent.
- Catalog FUN_ names: 0x00A643B0 PublishAlignment, 0x00BE5D40 GetWorldMapX, 0x00BE5D60 GetWorldMapY.
