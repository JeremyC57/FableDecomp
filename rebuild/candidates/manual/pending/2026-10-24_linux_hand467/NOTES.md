# Hand chunk 467 (CBuyableHouseDef .. CWorld): 38 of 40 byte-exact

All 38 sources grade MATCH (11) or RELOCATION_MATCH (27) through gall.sh. Their behaviour tests pass under g++ (tall.sh) and VC7.1 (tall.py). The call-target check (calls.py) shows every relocation going to retail's target: scalar `operator delete` 0xBFE9BC in the deleting destructors, `free` 0xBFEA14 in the malloc-CArray destructors, `memmove` 0xBFEAE6, and the DeleteCriticalSection and CloseHandle IAT slots. None of the 38 needed a `// Flags:` line.

## What matched (38)

- **CBuyableHouseDef / CCarryableDef / CLensFlareElementDef / CSpecialAbilitiesFireballSpellDef::Transfer** (0x004E7D97, 0x004DD924, 0x0046F727, 0x004656A6):
  - Each persists the def's members with one-argument `context.Transfer(member)` calls.
  - All four are built under `#pragma optimize("s", on)`. In a size TU every member address goes through eax, and the last one is a natural `add esi,N ; push esi`.
  - The order is the def's TRANSFER list, not the declaration order: IsScripted comes before MaxRentBags; Inclination before ReleaseDelay; Radius, Texture, Position, Colour.
  - Leaves need `class` types (V mangling).
- **STLport vector::clear** (0x0049BAE0 `vector<float>`, 0x008F9E90 `vector<char>`):
  - `erase(begin(), end())` over STLport 4.6 `__copy_ptrs` / `__copy_trivial`, written in the `(last == first) ? result : memmove(..)+n` form. This keeps the never-true `cmp eax,eax ; jne` and the zero-length memmove tail.
  - Both are modelled as real `std` templates with an explicit instantiation.
- **Deleting destructors** (0x00414FC0 CConsoleCommandBase, 0x009973C0 CMemoryDataOutputStream, 0x009D8140 CLoadingMemoryFailureHandler, 0x009FC700 CResourceList, 0x00C5EC00 the no-RTTI handle class, 0x008EFFD0 Coin):
  - The rule: an out-of-line (empty, or DeleteCriticalSection) virtual destructor defined in the TU emits the vtable and ??_G, and VC7.1 inlines it into ??_G.
  - Classes that declare no destructor (CDataOutputStream, Coin, CConsoleCommandParameters) drop their own vtable reset.
  - Bases with an inline `virtual ~X() {}` store their vtable.
  - Coin's CIntelligentPointer member sits at +0x25, so the pointer classes are declared under pack(1), as the real layout requires.
- **CWStringParser** (0x00A0C910 SkipWhitespace, 0x00A0C950 NextItemExists, 0x00A0CF10 ReadNextSymbol (private), 0x00A0D0A0 ReadNextChunkAsString):
  - A polymorphic class over `wchar_t*`, calling `iswspace` (0xBFEB90).
  - SkipWhitespace is a count loop that re-reads `String[StringPos + ++count]`.
  - NextItemExists is a top-tested `while (c != 0)`, which VC rotates, keeping two exits.
- **CSharedEntry::CSharedEntry** (0x00C37770, libpk, descriptive name):
  - Init list, then `table->m_entries.Add(this)` through slot 9.
  - The landed CSharedEntryTable::Acquire 0x00C2A3D0 calls it.
- **CWADFile** (0x00A39D20 GetFileInformation, 0x00A39D80 ctor):
  - VC7.1 has no NRVO, so the CFileInformation local is copied out with `rep movsd`.
  - The ctor's initialisers are scheduled before the vtable store, ahead of the direct Open call.
  - Both use the landed local polymorphic model, because engine/CWADFile.h makes the vptr a data member.
- **CWaterPatchMesh** (0x00BEAB20 RelocateData, 0x00BEB490 ctor):
  - RelocateData uses the landed rebase helper.
  - The ctor's C2DCoordI needs a memberwise user copy ctor. Without it, the bitwise copy sinks the Offset.Y store below the Load argument push.
- **CWaterPatchTesselatorBase<CTVertexWaterBackground / CTVertexSeaBackground>** (0x00B7EC40, 0x00B7EC60, 0x00B7ECF0, 0x00B7ED10, 0x00B7FCE0, 0x00B80100):
  - The functions are GetBridgeVertexBufferSize (the pointer difference divided by the element size, using a reciprocal multiply), GetBridgeVertexBufferAsCArray (the `GetCount() ? _M_start : (T*)this` select) and SetBridgeVertexBufferSize.
  - SetBridgeVertexBufferSize is STLport `resize(n, T())` inlined. The vertex's empty user ctor leaves the temporary unwritten.
  - All six come from one generator: `gen/tess.py`, with explicit member instantiation.
- **CAudioFormat::GetBytesPerSecond** (0x00C2D840, libpk, descriptive name):
  - The body is `(float)GetBlockAlign() * (float)m_sampleRate / (float)GetSamplesPerBlock()`.
  - The two callees get same-TU `__declspec(noinline)` bodies, so VC keeps ecx live across both calls.
- **STLport `swap<pair<unsigned __int64,bool>>`** (0x00A7D520, 0x00A7EF90 twins):
  - `__fastcall` swap over a pair with a memberwise user copy ctor.
  - The implicit operator= copies all 16 bytes, so the uninitialised padding of the temporary travels.
- **CEnginePrimitiveWeaponTrail::CWeaponTrailEntry ctor** (0x0076DE20):
  - A nested class. C3DVector needs a memberwise user copy ctor; the implicit copy makes a lea destination pointer plus an esi temporary.
- **CWideScreenMagicPauseEntities ctor** (0x00CBE09A):
  - `Game(game)`, then `Game->PauseAllNonScriptedEntities(true)` through GameCalls slot 379 (+0x5EC).
  - The slot name is the one the landed CDF_ShineLight::Main uses; the landed destructor 0x00CBE0B3 calls it with false.
- **CWitnessesSearchingDeed ctor** (0x006C5410):
  - Init list with an inline `CIntelligentPointer<T>(T* item) { SetPItem(item); }`: the base ctor 0xA01B10, then the CIntelligentPointer<CThingAICreature> vtable 0x0123E7E0, then SetPItem 0xA01B90.
  - No EH frame.
- **CWorld** (0x0049D930 SetCreatureGenerationGroupAsEnabled, 0x0049D980 ProcessEvent, 0x0049DA00 / 0x0049DA60 GetCurrentRenderCamera, 0x0049E590 SetSaveGameMarkerPos):
  - SetCreatureGenerationGroupAsEnabled is `if (enabled) mask &= ~group; else mask |= group;`.
  - ProcessEvent uses the dispatch table's ActionFunction pointer to member, written in the landed CWorld_Methods form.
  - In GetCurrentRenderCamera(info), `t` is selected with `Paused ? info.PausedCurrent.WFInterpolate : info.Current.WFInterpolate` and homed in the dead `info` slot.
  - In GetCurrentRenderCamera(), the 1.0f push is hoisted above the branch.
  - SetSaveGameMarkerPos is a C3DVector assignment into the unaligned +0x141, then `if (hero && !hero->Dead) angle = hero->PhysicsTC->GetAngleXY()`.

## Parked for Fable 5.1 (2)

- 00a0d0e0 CWStringParser::ReadAsInteger DIFFER(82v81): only the one 0xCC VC7.1 emits after the final noreturn `_CxxThrowException` call; the row cut stripped it, and every byte before it is identical | tried: CWParsedItem local + `throw CGenericException()` (landed ReadAsString/ReadAsSymbol twin form) | next: int3-after-noreturn, land against the 82-byte span
- 00a0d140 CWStringParser::ReadAsFloat DIFFER(88v87): only the trailing 0xCC after the final noreturn `_CxxThrowException` call; every byte before it is identical | tried: the ReadAsInteger form with `item.Type == 4` and the float value | next: int3-after-noreturn, land against the 88-byte span

## Skipped

None.

## Label corrections

- 0x0049BAE0 and 0x008F9E90 "CComponent::SetParentColour" are STLport `vector<float>::clear` and `vector<char>::clear`. The label was propagated across byte-identical bodies.
- 0x008EFFD0 "CLandscapeBackgroundSubNodeSplitBranch::`scalar deleting destructor'" is `Coin::`scalar deleting destructor'` (Coin vtable 0x01288238 slot 0).
- 0x00C37770 "CWStringParser::Init" is the libpk `CSharedEntry::CSharedEntry(CSharedEntryTable*, void*)` (descriptive name). The real Init is 0x00A0C8E0.
- 0x00A0D140 "CWStringParser::ReadAsInteger" is `CWStringParser::ReadAsFloat` (`?ReadAsFloat@CWStringParser@@QAEMXZ`; type 4, float result).
- Five "CWaterPatchMesh::`scalar deleting destructor'" rows belong to other classes:
  - 0x00414FC0 is CConsoleCommandBase.
  - 0x009973C0 is CMemoryDataOutputStream.
  - 0x009D8140 is CLoadingMemoryFailureHandler.
  - 0x009FC700 is CResourceList.
  - 0x00C5EC00 is the no-RTTI handle class with vtable 0x012B90D4 (landed CVtable_012b90d4_00c5ec50).
- The CWaterPatchTesselatorBase vfunc rows get real names:
  - vfunc_1 is GetBridgeVertexBufferSize.
  - vfunc_2 is GetBridgeVertexBufferAsCArray.
  - vfunc_4 is SetBridgeVertexBufferSize.
  - Each comes in a CTVertexWaterBackground instance (vtable 0x012A3334) and a CTVertexSeaBackground instance (vtable 0x012A334C).
- 0x00C2D840 "CWeaponDrawState::CalcBlockDamageReduction" is the libpk `CAudioFormat::GetBytesPerSecond` (descriptive name; no PDB).
- 0x00A7D520 and 0x00A7EF90 "CWeaponTrailEntry" are STLport `swap<pair<unsigned __int64,bool>>` (byte-identical twins).
- 0x0076DE20 "CWeaponTrailEntry" is the nested `CEnginePrimitiveWeaponTrail::CWeaponTrailEntry` constructor.
- Already landed: 0x0049E1D0 (landed as `CWorld_ProcessEvent_0049e1d0.cpp`) is Ego_r.pdb `CWorld::ReceiveLocalEvent`. The evidence:
  - It sits between PeekThingManager and GetChecksum, as ReceiveLocalEvent does in the PDB order.
  - It calls LocalActionFunction (+0x10).
  - The real `CWorld::ProcessEvent` is this chunk's 0x0049D980 (after PostLoadInit, ActionFunction +0x00).
