# Hand chunk 561 (CPersistContext .. CThing): 10 of 12 byte-exact

All 10 matched rows are RELOCATION_MATCH (no retail_asm rows). Every test passes under g++ (tall.sh) and VC7.1 (tall.py);
the vland dry run gives 10 wins, no RELOC_CONFLICT / DUPLICATE / IDENTITY.

## What matched (10)

- **CPersistContext::Transfer<T>** 0x004045C0 (bool), 0x00410620 (float), 0x004106F0 (unsigned long), 0x004AB640
  (CWorld::ESaveType). CheckCRC(name), then a switch over the persist mode (+0x18: 0 load text, 1 save text, 2 load binary,
  3 save binary, 4 reset) with `default: __assume(0)` (bare `jmp [eax*4+table]`), case bodies in source order 4,3,2,1,0.
  CPersistTraits<T> has no constructor and is block-scoped in each binary arm (retail homes it in a dead argument slot; a
  function-scope traits takes the key slot). The text arms build the 8-byte {vtable, Object} wrapper
  CPersistContext_TransferableComponent<T>, whose vtable's RTTI names T. Binary save is the inline
  TransferBinaryOut: WriteEBOOL for bool (explicit specialisation), WriteFloat for float, generic WriteSLONG otherwise.
- **CUserProfileManager::ApplyNewProfileValues** 0x0040CCA0. One inline `SetX(XNewProfile, false)` per option in retail
  order, then `if (NewProfileToUseWASD) ResetAssignedInputsWASD();` and `SetShowTargetingStatus(ShowTargetingStatusNewProfile,
  true)`, whose `if (save) Save()` becomes the tail jump. The setters must be the retail TU's own bodies (`if
  (UseNewProfileValues) XNewProfile = v; X = v; ApplyValues(); if (save) Save();`): inlined with v = XNewProfile the guarded
  store is a self-assignment, so VC7.1 drops it and its test, and the by-value parameter keeps every bool copy in al. Plain
  `X = XNewProfile;` rotates al/cl/dl. Containers are modelled as `unsigned long[n]` so the members keep 4-byte alignment.
- **NUISystem::CSprite::~CSprite** 0x0041AB40. The user body is `Handle.Clear();`. The landed CText dtor 0x0054F6E0 model
  gives the rest: forceinline CEnginePrimitiveHandle::Clear (null-safe GetReferenceCount() == 1, NotifyReleased, Reset(0)),
  then the member dtors (~CCountedPointer<CTexture>, the inline virtual ~CEnginePrimitiveHandle) and a tail jump to
  ~CChangingStateComponent.
- **NUISystem::CManager::GetCorrectComponentInterface** 0x0041CD93. A switch with a separate `result = component; break;`
  per accepted type (0..43 except 26, 28-31, 37) keeps the direct dword jump table (one shared body gives a byte-index
  table). `#pragma optimize("s", on)`: the TU is packed at unaligned addresses, and /Os drops the alignment nop.
- **CDisplayEngine::SetAboutToRun** 0x00434CD0. Inline CFadeInFadeOutBase::Open (out of line at 0x00434840) needs its
  stores in the order FadeInTime, FadeOutTime, Active, Closing, Opening, OpenTimer, CloseTimer, ToColour. Other orders cost
  3-5 instructions here, though every order compiles the same out of line. Then Update(0.1f), inline Close(), the three
  fade fields, and EndProfile(1).
- **CWorld::UpdateSaveGameLoad** 0x004A2F10. A four-state switch. The by-value CRGBColour argument needs CRGBColour's user
  copy constructor (as in the landed CTCWife::HaveSex) so it is built in its stack slot. GetTC<CTCRegionDisplay> must be
  `__forceinline`: VC7.1 declines to inline it here otherwise (as in the landed TriggerSetPlayerAreaName). Retail uses the
  out pointer unchecked. The getters `UseNewProfileValues ? XNewProfile : X` give the integer spill through [esp+14h].
- **CThing::~CThing** 0x004C8310. CClass's destructor is implicit: a user-declared `virtual ~CClass() {}` stores the CClass
  vtable before the tail jump to ~CBaseObject, and retail does not. The STLport list sweep is `it = erase(it)` else `++it`.
  The component loop uses STLport `bitset::reset(pos)` with its no-exceptions range check (`puts("bitset"); abort();`, the
  cold block placed last), then `delete` and a null store. `clear()` is STLport erase(begin(), end()). The CDefPointer
  release reads the pointer once into a local. The member dtors follow in reverse order.

## Parked for Fable 5.1 (2)

- 00405a60 CPersistContext::Transfer<unsigned char> DIFFER(208v308): no instruction differs; the catalog row is over-spanned (the true function, jump table included, ends at 0x00405B30, where a jmp thunk and then small getters 0x00405B40..0x00405B90 follow), and tools/pfx.py finds 0 mismatches over the 208-byte extent with relocations masked | tried: the Transfer<T> template (same source as the matched rows), movzx through generic WriteSLONG | next: lane gap: catalog extent -- re-split the row at 0x00405B30 and land differ/00405a60.cpp as is
- 0044bb00 CPersistContext::Transfer<unsigned __int64> DIFFER(224v261): no instruction differs; the row runs on into the 2-argument Transfer<unsigned __int64> at 0x0044BBE0, and the 224-byte extent is byte-exact with relocations masked | tried: the Transfer<T> template with WriteUHUGE | next: lane gap: catalog extent -- re-split at 0x0044BBE0 and land differ/0044bb00.cpp as is

## Skipped

None.

## Names

### Proven (two sources each; exact PDB spelling, confirmed in the objects with objdump)
- 0x004045C0 `??$Transfer@_N@CPersistContext@@QAEXPBDAA_NAB_N@Z`, 0x00405A60 `??$Transfer@E@...`, 0x00410620 `??$Transfer@M@...`,
  0x004106F0 `??$Transfer@K@...`, 0x0044BB00 `??$Transfer@_K@...`, 0x004AB640
  `??$Transfer@W4ESaveType@CWorld@@@CPersistContext@@QAEXPBDAAW4ESaveType@CWorld@@ABW412@@Z`: the RTTI of each text-arm
  wrapper vtable (0x0122D6C4 / 0x0122DCD4 / 0x0122DFE0 / 0x0122E004 / 0x01231C80 / 0x01238C74) names T, and Ego_r.pdb has
  each instantiation.
- CPersistTraits<T>::TransferBinaryIn 0x00403EB0 (bool), 0x00405940 (E), 0x0040EFB0 (M), 0x0040F8A0 (K), 0x0044B890 (_K),
  0x004AA270 (CWorld::ESaveType, FableWin.pdb): each is the binary-load arm of its Transfer, and the PDBs have the
  `QAEXAAVCMemoryDataInputStream@@AAT@Z` form.
- CPersistContext::CheckCRC 0x00404500, TransferObjectSaveText 0x009BAD60, TransferObjectLoadText 0x009BBA40 (protected IAE;
  they take CPersistContext_TransferableComponentGenericType&).
- CDataOutputStream::WriteEBOOL 0x00993E30, WriteSLONG 0x00993EB0, WriteUHUGE 0x00993EC0, WriteFloat 0x00993EE0 (the
  writer run follows Ego_r declaration order; the Transfer arms pick them by T).
- CUserProfileManager::ApplyNewProfileValues 0x0040CCA0, ApplyValues 0x00407370 (protected), ResetAssignedInputsWASD
  0x00408820 (WASD control-scheme strings), Save 0x0040A740 (every setter calls it on `save`; the body writes the profile
  file through CAFile; the old binding `?Apply@` names a different Ego_r method), Get 0x0040D2A0.
- `??1CSprite@NUISystem@@UAE@XZ` 0x0041AB40 (three CSprite@NUISystem vtable stores; PDB), ~CChangingStateComponent
  0x0052CCF0 (RTTI base list), ~CBaseClass 0x0099A2E0, vtables 0x0122F598 CEnginePrimitiveHandle (global),
  0x0122F5D4/0x0122F5AC/0x0122F5A4 CSprite@NUISystem for the CUIInterface/CObserver/CActionParam subobjects.
- `?GetCorrectComponentInterface@CManager@NUISystem@@QAEPAVCComponent@2@PAV32@@Z` 0x0041CD93; CComponent::GetType slot 65.
- `?SetAboutToRun@CDisplayEngine@@QAEXXZ` 0x00434CD0; CFadeInFadeOutBase::Open 0x00434840 / Update 0x00434870 / Close
  (inline); NProfileTimer::EndProfile 0x009D8250 and StartProfile 0x009D8240 (Ego_r.pdb's only __fastcall void(long) /
  void(const CCharString&, long) pair, and ActivateWorld 0x0049F180 calls them in that pattern).
- `?UpdateSaveGameLoad@CWorld@@IAEXXZ` 0x004A2F10 and its callees: StartScreenFadeOut 0x00434C00, EndScreenFadeOut
  0x00434C90, IsScreenFullyFadedOut 0x004A9210 (QBE), IsHeroStateFileValid 0x0049D4E0 (IBE), LoadGameStateInternal
  0x004A21F0 / LoadHeroStateInternal 0x004A2D70 (IAE), CAFile::SplitPath 0x00997620, operator+ 0x0099BE70,
  CWorldMap::GetLoadedRegion 0x004FB150 / GetRegion 0x004FC180, CRegion::GetDisplayName 0x006BBF20,
  CPlayer::GetPControlledCreature 0x00487DC0, CTCRegionDisplay::SetCurrentAreaName 0x00639BA0,
  NProgressDisplay::StopProgress 0x009E9F20, CIEngine::StartNewScene (slot 55: the CIEngine 0x012A0D24 and CEngine
  0x012A0F3C RTTI vtables hold ?StartNewScene@ there), CWorld::PeekWorldMap (slot 13), inline SetAsPaused /
  GetDisplayEngine / GetSoundVolume / GetDialogueVolume / GetTC<CTCRegionDisplay> (PDB names; Ego_r has them out of line).
- `??1CThing@@MAE@XZ` 0x004C8310 (protected virtual; vtable 0x01239ABC RTTI .?AVCThing@@), ~CBaseObject 0x0099A430 (MAE),
  CBaseIntelligentPointer::GetPItem 0x00A01B50 (returns CBaseObject*) and its dtor 0x00A01C10, CClass::Unregister
  0x00A37160 (`?Unregister@CClass@@QAEXAAVCClassFactory@@@Z`: one of only two CClass(CClassFactory&) methods in the PDB,
  and the body unregisters: OnDeleteClass, then the type/class lists are unlinked), static CThing::TCsToBeRemoved 0x013B8A24
  (the PDB names its initializer/atexit destructor; the landed clear 0x004CC370 proves the element type).

### Probable
- CPlayerManager::GetMainPlayer 0x00449970 as the non-const `QAEAAVCPlayer@@XZ`: the PDB has const and non-const forms, and
  this caller holds a non-const pointer.

### Descriptive (no PDB data/enum names exist)
- Globals: g_ProgressDebugDisplay 0x013B85F0 (CProgressDisplay::StartProgress 0x00499AA0 copies it into DebugDisplay
  +0x78) and g_ProgressDebugDisplaySaved 0x013B89B8 (only this function touches it); g_InitialFadeActiveMirror 0x01375CDC,
  g_pSoundSystem 0x013B8394 (as CSoundManagerLH*, the landed majority spelling), g_pThingManager 0x013B8A1C, g_thingCount
  0x013B8A20 (landed names kept).
- CPersistContext::EMode enumerators; empty enums CWorld::ESaveType, NUISystem::EType, EEngineSprite2DFlag, ETCInterfaceType.
- Positional virtual placeholders: CUnprovenSlot_NNN (CComponent 1..64, CIEngine 0..54, CWorld 0..12),
  CUnprovenSlot2_012a3d60 (CEnginePersistentPrimitiveBase slot 2 = _purecall), CDefPointeeBase CUnprovenSlot0/1, the
  CSoundManagerLH v000..v045 and CTCBase v01..v20 fillers (landed model style).

## Label corrections
- 0x004045C0 / 0x00405A60 / 0x00410620 / 0x004106F0 / 0x0044BB00 / 0x004AB640: catalog "Transfer<signed char>"
  was propagated across the byte-identical instantiations. Each is the instantiation its RTTI names.
- 0x00434840 catalog "CEngine::StartNewScene" is CFadeInFadeOutBase::Open. CEngine::StartNewScene is the slot-55 virtual
  0x00B239A0.
- 0x004FB150 label `_global::GetMember_dword0x9c_004fb150` is CWorldMap::GetLoadedRegion() const.
- 0x00487DC0 label `CPlayer::GetHeroCreature` is CPlayer::GetPControlledCreature() const.
- 0x0041AB40 catalog `CSprite::~CSprite` is NUISystem::CSprite. 0x0041CD93 `CManager` is NUISystem::CManager.
- 0x00A37160 landed as CClass_Retail::UnregisterFromFactory. The PDB method is CClass::Unregister(CClassFactory&).
- 0x013B8394 label `g_pSoundManager` disagrees with the landed `g_pSoundSystem` (no PDB data symbol either way).
- NAME_FIXES.tsv has 55 rows for the coordinator: each retail address with its PDB spelling and the wrong spellings bound
  to it in rebuild/oracles/reloc_bindings.tsv.

## Header debt
- engine/CWorld.h, engine/CClass.h, engine/CBaseObject.h, engine/CIEngine.h, engine/CTCBase.h,
  engine/CIVCountedPointeeBase.h, engine/CObserver.h and engine/CSprite.h all store the vptr as data (`void* __vftable`).
  Rows that make virtual calls on these classes, or subclass them, model them locally. This is why vland warns
  SHARED_HEADER_AVAILABLE on 0041ab40 (CIVCountedPointeeBase, CObserver), 004a2f10 (CIEngine) and 004c8310 (CTCBase).
- engine/CManager.h is a global `struct CManager`; the leaf needs NUISystem::CManager.
- There are no headers for CPersistContext, CUserProfileManager, NUISystem::CComponent, CThing or CMainGameComponent.
- 004a2f10 uses engine/CDisplayEngine.h, CRegion.h, CWorldMap.h, CPlayer.h and CPlayerManager.h through the rename trick.
  00434cd0 uses CDisplayEngine.h and CFadeInFadeOutBase.h. The Transfer rows use CMemoryDataInputStream.h and
  CDataOutputStream.h.

## Review at landing
- finish.py splits line 1 of 004ab640 at the template's inner `::`, so the payload has module `CPersistContext::Transfer<CWorld`
  and name `ESaveType>`. The leaf is right. Land it as module CPersistContext, name `Transfer<CWorld::ESaveType>`.
- 004045c0 replaces the landed asm_bake `CPersistContext_TransferSignedChar_004045c0.cpp` (wrong name and grade).
- rebuild/oracles/reloc_bindings.tsv has 10 rows whose target is `-` (lines 10-14 and more). The stock
  reloc_identity.bindings_table loader throws on the first of them. That leaves the table partial, so RELOC_CONFLICT goes
  unchecked for every row after the first (the stock dry run printed RELOC_CHECK_ERROR on 004045c0 only). I re-ran the dry
  run through tools/vland_wrap.py, which skips the bad rows: still clean (tmp/vland2.log).
- 0040cca0 emits the inline setters as COMDATs under their PDB names. Their retail out-of-line copies (0x0040AF90..) are
  other catalog rows, so the obj defines them without claiming those addresses.
- 004c8310 binds `_puts` / `_abort` to the MSVCR71 import thunks 0x00BFEAF2 / 0x00BFEAEC (RELOC_ALIAS warning, expected).
- 004a2f10 RELOC_NAME warnings come from the wrong labels listed above.
