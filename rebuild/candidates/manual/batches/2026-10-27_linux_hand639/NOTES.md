# Hand chunk 639 (CLocalDetailPrimitiveMeshZSpriteBatch .. CTCVillage): 5 of 10 byte-exact

Chunk 639 is the 10-row parked-retry chunk (CHUNK_BRIEF.md overrides PROMPT.md's "of 40"). Each row started from its old
best attempt, which was the same length as retail. All 5 matches are RELOCATION_MATCH. No row is retail_asm and none is
SKIP. tall.sh (g++) and tall.py (VC7.1) both print PASS for all 5 tests.

## What matched (5)
- **CLocalDetailPrimitiveMeshZSpriteBatch::SetupPrimitiveDesc 0x00BF9FA0** (325): builds the z-sprite batch's
  CAddPrimitiveDesc: bounding sphere and box, the largest object scale, the type's z-sprite mesh, the fade distances, the
  entries and the alpha ref. Lever: the PDB `Peek*` type accessors (PeekFadeStart / PeekFadeEnd / PeekZSpriteMesh /
  PeekZSpriteFadeStart / PeekZSpriteFadeEnd / PeekAlphaRef) plus the PDB ctor parameter order, with CEngineFadeDistance
  passed by value and the ctor `__forceinline`. Together they fix the esi/edi swap. The leaf is private (AAE) and returns
  `NEnginePrimitiveManagerRepeatedZSprites::CAddPrimitiveDesc`.
- **NScript::CQ_FireHeartScript::DisableFollowers 0x00D3A050** (330): collects the hero's followers, resizes the handle
  vector and takes scripted control of each follower in turn (releasing any stale handle first, retrying a script frame at a time), then fades it out and hides its emote icon. Lever: a named begin() pointer,
  `CScriptGameResourceObjectScriptedThingBase* handles = FollowerHandles.begin(); if (handles[i].IsValid())
  handles[i].Release();`, which gives retail's `mov eax / lea`. The handle class is row-local (ctor 0x007E72A0 and virtual
  dtor 0x007E74D0 out of line), as in the landed 0x00D49D50 and 0x00CD3D2E. The shared header expands the implicit
  ctor/dtor inline (410 bytes).
- **CInputManagerDX::CInputManagerDX 0x00A60050** (333): builds the input manager, creates the DirectInput8 interface on the
  application instance, then creates the DirectX joystick (kept in the joystick list), keyboard and mouse devices that the init record asks for. Lever: an inline
  `CSystemManager::GetWinInstanceHandle()` accessor (+0x90) plus the real ctor signature `(const CInputManagerInit&, bool)`.
  The old attempt's SInputDeviceConfig/HWND model was wrong. RTTI bases are used: CInputManager : CBaseClass,
  CKeyboard/CMouse : CBaseClassNonCopyable, and CJoystick has none.
- **NScript::CQ_TraderEscortScript::CDarkwoodRockTroll::Main 0x00E03D30** (335): an alive filter on Me, then the troll is
  set on the hero, then the script waits a frame at a time until its health is <= 0. It then sets the quest card objective
  TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03 (BarrowFields) and clears ParentClass->ShownEarthTroll (+0x55, as in the
  landed Init). Lever: the real CGameScriptInterface slot signatures (GiveThingBestEnemyTarget, GetHero, GetHealth,
  SetQuestCardObjective, GetActiveQuestName) from engine/CGameScriptInterface.h. These fix the outer vtable-fetch order.
- **CGameScriptInterface::SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden 0x00894EC0** (335): collects dead creatures,
  TC 0x69 things, the two def-named drop bags and TC 0xCC objects, then calls SetAsDrawable(!hidden) on every live one.
  This uses chunk 638's lever. The same-TU stand-in bodies of the GetAllThings<T, list<CThing*>, F> specialisations
  (PDB spellings) take the HasTC / HasDefName filter's address, but the empty None filter's stand-in takes nothing. The
  None filter is a named local with no ctor, so VC7.1 homes it in the dead `hidden` slot.

## Parked for Fable 5.1 (5)
- 0073acc0 CTCCreatureModeManager::AddMode DIFFER(331v331): retail emits the early-return `xor al,al` at the top of the join block, before `pop edi`; ours emits it between `pop ebp` and `pop ebx` | tried: IsModeActive inline/__forceinline, shared CCountedPointer.h (dtor reloads Info, 353), local CCountedPointer with/without cached block, same-TU push_back body, single-exit if form (335), plus the old sweep's inverted big-if / bool result / named found | next: lane gap: epilogue scheduling (the true path's `mov al,1` takes the same slot ours uses)
- 00641ae0 CTCLight::DrawFlare DIFFER(331v331): flare-block registers differ: retail uses halo=edx, thing=eax, uid lo/hi=ecx/eax; ours uses ecx, edx, eax/edx | tried: IsOverridden/GetHaloGraphic accessors, PDB flare ctor signature (ulong64 id to CEnginePrimitive base), CDefPointer<const CLightDef>, named ulong64 id, class keys | next: model CTCGraphicAppearance's primitive-add call with its PDB signature and the CEnginePrimitive id as a by-value ulong64 temporary
- 00ba0fc0 CEnginePrimitiveManagerStaticMeshesBase::AddStaticMesh DIFFER(333v333): the `mov [ebp+0xc],ebx` flags store is scheduled right after its load; retail stores it after Alpha | tried: AddSprite 0x00BAE160 model, PDB hierarchy (OcclusionBase -> MeshBase -> StaticMesh, ctor 8/10), AddPrimitive(CEngineInternalPrimitiveBase&, ulong), Data.Reset, SetNonPersistent setter, flags statement after Alpha (326, zero reg flips) | next: lane gap: store scheduling
- 00b92300 CEngineSceneGridCell::CreateOcclusionGroup DIFFER(335v335): `mov edi,[esp+0x14]` / `lea ebx,[ebp+edi*8+0x24]` (layer edi, handle ebx, all pushes first) vs ours `mov ebx,[esp+0xc]` / `lea edi,...` (pushes interleaved) | tried: __forceinline PDB helpers (Clear, SetData, Reset, AddPrimitive, InvalidateAll), null-safe GetRefCount, early return vs if-block (if-block puts layer in edi but drops the handle register), handle reference vs re-index | next: lane gap: callee-saved register tie
- 0065be6c CTCVillage::UpdateHornTasks DIFFER(336v336): `this` is in ebx and the def temporary / find_if result / new'd task are in edi in retail; ours swaps them | tried: PeekWorld + CWorld::PeekGameTimeManager + CGameTimeManager::IsTimeBetween const (fixed the [eax+0x1c] load), CIsHornTaskWithThisHorn, STLport names, private leaf, engine/CDefPointer.h (registers right but `cmp [obj+4],0` after each dec, 344), the IsCurfewTime 0x00658DB3 def model (exact release, swap), release variants | next: a def release that both keeps `dec ; jne` and is header-shaped (the register choice follows the release form)

## Skipped
None.

## Names (proven vs descriptive)
- Every row's own leaf is a PDB spelling confirmed with objdump -t (see STATUS.tsv). 0x00BF9FA0 and 0x00A60050 replace
  wrong spellings from the old attempts.
- CTCCreatureModeManager::CModeBase slots, from vtable 0x012673CC and two sources:
  - 1 GetType() const is _purecall in the base; FableWin.pdb's CModeBriarRoseTeleportInvulnerable overrides it as UBEKXZ.
  - 2 AnimUpdate, 3 FrameUpdate, 4 GetAnimFilter, 5 OnActivate, 6 OnDeactivate (body `Active = false`), 7 GetMovementSpeed.
  - 8 IsIdleMode (the remaining bool-const name once 9 and 11 are pinned) and 9 IsAllowingPlayerSpecialMovementMoves (the CModeDrained vtable 0x01268234 slot 9 = 0x00741480,
    which is landed with that name).
  - 10 IsHoldWeaponWithBothHands (landed).
  - 11 IsRemoveOnDeactivate: CModeBriarRoseTeleportInvulnerable overrides only this bool virtual in FableWin.pdb, and
    its slot 11 is 0x008C36A0 `mov al,1`.
  - The landed RemoveMode / ClearModes spell slots 5/6 as OnResume/OnLeave. Those spellings are descriptive and wrong
    (see Review at landing).
- Descriptive or unproven names:
  - The ETCInterfaceType enumerators 0x69 / 0xCC (0x00894EC0) are cast numbers.
  - The `g_framesPerSecond` spelling for 0x01375550 is the landed RemoveMode's. reloc_bindings has more than 30 spellings
    for this address.

## Label corrections
- 0x008AC5D0 / 0x008AC660 / 0x008AC700: the manifest's "GetAllThings<CThingAICreature/CThingMarker, vector, ...>" labels
  are propagated. They are the GetAllThings<CThingDeadCreature|CThingObject, list<CThing*>, None|HasDefName|HasTC>
  instantiations.
- 0x00CD3CCB: labelled "CArray push_back"; it is vector<CScriptGameResourceObjectScriptedThingBase>::resize(n, const T&).
- 0x0049D870: labelled GFGetPreMainMemoryUsed; it is CWorld::GetFrame (static __fastcall).
- 0x007421C0: labelled "ModeList_PushBack"; it is
  list<CCountedPointer<CTCCreatureModeManager::CModeBase>>::push_back (operator new 0x10, copy, link before the sentinel).
- 0x006BB390: labelled "Game_IsTimeOfDayInRange"; it is CGameTimeManager::IsTimeBetween(long, long) const.
- 0x00686A80: labelled "CViewBase::GetDisplayEngine"; it is CTCBase::PeekWorld() const.
- 0x0065F0FA: labelled "CTCActionUseBuyHouse::OnCreate"; it is CIsHornTaskWithThisHorn::CIsHornTaskWithThisHorn(CIntelligentPointer<CThing>).
- 0x0065FFCA: labelled "CGuiControlTree::SelectNextEntryByName"; it is the STLport find_if<list<CCountedPointer<CVillageTaskHorn>>::iterator,
  CIsHornTaskWithThisHorn>.

## Header debt
- These headers flatten the vptr, so their rows use local models:
  - engine/CTCBase.h, CLightDef.h, CMovableResource.h (vland SHARED_HEADER_AVAILABLE on 0x00BF9FA0) and CJoystick/CMouse/CKeyboard.h.
  - The CEngineInternalPrimitive*.h family, CEnginePrimitiveManagerStaticMeshesBase.h and CTCVillage.h (`#pragma pack`
    flattened).
  - engine/CThing.h is quarantined, so 0x00894EC0 models CThing locally (slot 28 SetAsDrawable, dead bit +0x91).
- engine/CCountedPointer.h's dtor (it re-reads Info and zeroes Data/Info) does not reproduce the inline release of
  AddMode's by-value argument (0x0073ACC0, `dec [Info] ; jne`). The parked source uses a local CCountedPointer, as the
  landed AddMode(long) 0x0073AE10 does.
- engine/CDefPointer.h / CIVCountedPointeeBase.h: `long Release() const { return --IVRefCount; }` compared against 0
  compiles to `dec ; cmp [obj+4],0`. Retail 0x0065BE6C and the landed IsCurfewTime 0x00658DB3 show `dec ; jne`.
