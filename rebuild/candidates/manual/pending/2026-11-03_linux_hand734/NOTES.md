# Chunk 734 -- CWorld: seven large rows (UpdateFrame, PreparePrimitivesForEngine, the save/load bodies, Init)

Result: 1 matched (UpdateFrame, RELOCATION_MATCH, vland PASS on the final full mirror), 3 parked with their best source in
differ/ (PreparePrimitivesForEngine, AutoLoadRespawn, LoadQuestsFromFile), 1 blocked by header debt (Init), 2 not attempted
(SaveGameStateInternal, LoadGameStateInternal: read and mapped, no budget left for a 2.8-2.9 KB draft after the resumed run).

| Row | Function | Result | Attempts | Residue / lever |
|---|---|---|---|---|
| 0x004A5A40 | UpdateFrame | RELOCATION_MATCH | 6 | bullet-time reference scoped to the else arm of the full-update choice |
| 0x004A1CB0 | PreparePrimitivesForEngine | DIFFER(1331v1330), 72 lines | 18 | cross-product operand order, SetInverse translation order, box-copy interleave |
| 0x004A5E90 | AutoLoadRespawn (/GS; catalog "AutoLoadRegionLeaving") | DIFFER(1722v1722), 1 pair | 10 | manual-save copy hoists one `lea ecx,[esp+10h]` above `je` |
| 0x0049F4C0 | SaveGameStateInternal | not attempted | 0 | -- |
| 0x004A0D90 | LoadQuestsFromFile | DIFFER(2738v2733), 30 lines | 10 | token PStringData not kept in esi across the first compare |
| 0x004A21F0 | LoadGameStateInternal | not attempted | 0 | -- |
| 0x004A6E30 | Init | blocked (header debt) | 0 | CWorld scoped_ptr / CCountedPointer members untyped |

## Matched
- **0x004A5A40 UpdateFrame** (src/004a5a40.cpp, test/004a5a40.test.cpp). Needed retail-branch members across 18 headers (see
  overlay). The last diff (edi kept as a zero register, PBulletTimeManager reloaded) went when the `CBulletTimeManager&` local
  moved inside the else arm: `if (IsNonScriptedEntitesPauseMode()) fullUpdate = true; else { CBulletTimeManager& b =
  GetBulletTimeManager(); fullUpdate = !b.IsBulletTimeActive() || b.IsTimeForFullUpdate(); }`. Local classes with no header
  (CAtmosProcess, CMusicManager, CMessageEventManager, CScriptConversationManager, CGameTimeManager, CInterpolationInfo,
  CEngineManager, CTBaseSingleton<T>) carry member functions only; header debt for the coordinator. The three data globals
  (0x013B8615 / 0x013B89C0 / 0x013B89D3) have no PDB data symbol: descriptive names.

## Parked
- **0x004A1CB0 PreparePrimitivesForEngine** (best pp17, 72 diff lines, frame 0x184 now equal). Levers that worked: same-TU
  bodies of GetAllLeafThings<CThingBuilding> (retail 0x004AD700) and GetDef<CThingBuildingDef> (0x004AB840) keep the vector in
  registers and drop the CDefPointer null check; __forceinline PreScale / SetInverse (FableWin-only inlines, owner rule
  2026-11-01); a named weather-mask local; the translation row assigned first from a `const C3DVector&` (350 -> 77 -> 72 lines).
  Residue: (1) the inlined Forward ^ Up loads Up first in the Y second / Z first products (retail), ours Forward first --
  operand respelling does not move it (the landed 0x0049CE00 model needed a free CrossProduct(a, b) with reference parameters;
  not used here because FableWin names ??TC3DVector, the member operator); (2) the SetInverse translation terms: retail
  evaluates E41 as X,Y,Z (register operand first), E42 Z,Y,X, E43 X,Z,Y; every single-expression spelling compiles Z,Y,X for
  all three, and `+=` statements are worse; (3) the mesh bounding-box copy interleaves loads and stores one ahead in retail.
  FableWin.pdb lists the inlines this row uses first (CMatrix3x4(EMatrix34Init), PostTranslate, HasPhysicsMeshObject); the
  identity-ctor + Set + PostTranslate form (pp10) is worse (385 lines), so the real SetInverse / translation form is still open.
- **0x004A5E90 AutoLoadRespawn** (best al1 = al2 with /GS). Name: the chunk lists it as AutoLoadRegionLeaving, but the catalog
  gives that name to 0x004A34F0 too; Ego_r.pdb sizes (AutoLoadRespawn 1643 B / 464 insns, AutoLoadRegionLeaving 539 / 148)
  against retail (1722 / 484 here, 564 / 154 at 0x004A34F0), FableWin's size order and the body (checkpoint save between the
  quest screens, then manual / quest-start / plain autosave by MostRecentSaveType) all say AutoLoadRespawn. The whole of the
  landed LoadGameState 0x004A3200 is expanded four times through AutoLoadInternal (retail 0x004A3730 is `jmp 0x004A3200`); a
  plain same-TU definition does it, no __forceinline. Residue: in copy 2 (MostRecentManualSaveName, the only copy whose name is
  not a temporary) VC7.1 cross-jumps `lea ecx,[esp+10h]` above the `je` to the path destructor; retail keeps it at the target.
  Tried: loaded flag over the arms, early-return / bool-return manual arm, LoadGameState called directly (same bytes), early
  `return false` in LoadGameState. Next: a different statement shape around copy 2 only (e.g. the manual arm last), or the
  LoadGameState arms as one shared queue tail.

- **0x004A0D90 LoadQuestsFromFile** (best lq2, 2738 v 2733, 30 lines). First draft was already the right size (2733 v 2733);
  CCharString::EqualsCaseInsensitive given its inline `_stricmp` body (retail expands it; no out-of-line copy anywhere; no landed
  caller) fixed the TRUE test. Residue: retail loads the command token's PStringData into esi, calls
  CBasicString<char>::operator==(const char*) for "AddQuest" and reuses esi for "AddTestQuest"; on the null path both literal
  tests are threaded together. Ours reloads the pointer after the call and re-tests null (+5 bytes). Tried: const token, a
  const-reference-bound token, a bool local for the first test, if + do/while, `else { if }`, the forceinline text-== opt-in,
  and (probe, reverted) the `if (PStringData) ... ; return strcmp` body: none moves it. Next: find what keeps the token
  un-aliased in retail (e.g. the token read through a different ReadAsStringUntilString form, or the compare through a
  `const CBasicString&` local).
- Needed: CWorld InitialActiveQuests (CArray<CCharString>), RegisteredQuests (vector<CCharString>), ActiveTestQuests
  (vector<CTestQuest>) typed; new engine/CTestQuest.h (Ego_r layout); CQuestManager::RegisterQuest; FABLE_RETAIL_CCHARSTRING
  (the literal -> const CCharString& conversion).

## Blocked / not attempted (groundwork for the next agent)
- **0x004A6E30 Init is blocked by header debt**: every step is `scoped_ptr<T>::reset(new T(...))` (swap, then the virtual
  deleting destructor in place or an out-of-line boost::checked_delete<T> 0x004ADD30 / 0x004AE080 / 0x004AE810) or
  `CCountedPointer<T>::Reset(new T)` (0x004ADF80, 0x004ADAE0, 0x004A9A80). The CWorld retail branch flattens those members to raw
  pointers (PWorldMap_ptr, PMessageEventManager_Data/_Info ...), so the resets cannot be written without stand-ins or pointer
  casts. It needs (1) the CWorld retail members typed as boost::scoped_ptr<T> / CCountedPointer<T> under their PDB names
  (PWorldMap, PEnvironment, ...) with boost::scoped_ptr::reset (Boost 1.30: `this_type(p).swap(*this)`) and
  boost::checked_delete, which respells every CWorld-retail includer (16 landed rows + this chunk's three); (2) polymorphic
  retail branches for CEnvironment (deleting dtor slot 0), CNavigatorManager (slot 0, AddNewClassType slot 1), CGameCameraBase /
  CGameCameraManager (deleting dtor +0x60). Ctor signatures (Ego_r.pdb): CWorldMap(const CMainGameComponent&, CWorld&,
  const C2DBoxI&, unsigned long) with C2DBoxI(0, 0, 0x2000, 0x2000) and 5; CGameTimeManager(CMainGameComponent&, CWorld&);
  CEnvironment(const CGameDefinitionManager&); CNavigatorManager(unsigned long 4, unsigned long* seed (Component + 0x1613C:
  GetLocalSeed), const CNavigatorManagerExternalInterface&) with a stack CGameNavigatorManagerExternalInterface(const CWorld&)
  (vtable 0x01238C58); AddNewClassType(CClassTypeInit(type 0x0129CBA4 / 0x0129CB44, 0, false, "Navigator A Star" / "Navigator
  flyer")); CFactionManager(const CWorld&); CCombatManager(CWorld&) then ResetDialogue; CMessageEventManager(const CWorld&);
  CThingSearchTools(CThingManager&, CWorldMap&); CGameCameraManager(const CWorld&); CBulletTimeManager(CWorld&, CPlayerManager&);
  COpinionReactionManager(const CWorld&); CScriptConversationManager(); CGameCameraBase(CWorld&); PCurrentGameCamera =
  PGameCameraManager; InitMeshBank; NParticleEngine::SetMeshBank / SetGraphicBank; NAnimationEvents::InitialiseManagers /
  LoadAnimationEvents(true); NUISystem::CManager::GetInstance()->SetMeshBank(CCountedPointer by value) / SetInputManager
  (GFGetSystemManager()->[+0x58]) / SetInput(1); NSpeechGainManager::Initialise(*this); CSkeletalMorphResourceManager::Instance
  created on demand then PreloadBoneConfigs. Each step: NProfileTimer::StartProfile(name, 0) and/or
  NProgressDisplay::DisplayProgress(name, -1.0f, false, false) (the exact per-step pairing is in dis/004a6e30.txt).
- **0x0049F4C0 SaveGameStateInternal** (2825 B, `and esp,-8` frame): StartAutoSaveProgress, CDiskFileWin32 + CMemoryDataOutputStream
  (ReserveSize), CPersistContext Init / StartSection / EndSection, Transfer<CCharString/long/bool/unsigned/ESaveType/CWideString/
  float/double>, GetLoadedRegion / GetRegion / GetDisplayName, CThingManager / CPlayerManager / CQuestManager / CWorldMap /
  CFactionManager SaveGameState, WriteString, GetLength. Shares the persist helpers with LoadGameStateInternal: one TU.
- **0x004A21F0 LoadGameStateInternal** (2931 B, /GS): CUserProfileManager::VerifySignature, StartProgress, PrepareForLoad,
  DeactivateAllQuests, CDataInputStream / CFileDataInputStream / ReadZlibCompressed / OpenMemorySource, CPersistContextReadSection,
  Transfer<...>.
- **0x004A0D90 LoadQuestsFromFile** (2733 B): ClearRegisteredQuests, CStringParser (NextItemExists / SkipWhitespace /
  ReadAsStringUntilString / SkipPastNextItemIfIs / ReadAsInteger), _stricmp, RegisterQuest 0x004B2850, CTestQuest, vector growth.
- **0x004A6E30 Init** (2259 B, EH frame `push -1`): NProfileTimer::StartProfile + DisplayProgress per manager, operator new +
  ctor + scoped_ptr reset (checked_delete<T> 0x004ADD30 / 0x004AE080 / 0x004AE810) and CCountedPointer resets for CWorldMap,
  CGameTimeManager, CEnvironment, CNavigatorManager (CGameNavigatorManagerExternalInterface vtable 0x01238C58), factions,
  CCombatManager, CThingManager, CMessageEventManager, CThingSearchTools, CGameCameraManager, CBulletTimeManager,
  COpinionReactionManager, CScriptConversationManager, CGameCameraBase; InitMeshBank, NUISystem::CManager set-up.

## Header overlay (hdr/; hdrcheck2: default=same for every changed file, each change behind its class's FABLE_RETAIL_<X>)
- engine/CWorld.h (FABLE_RETAIL_CWORLD): fable_containers.h + fable_string.h includes; forward enums EPlayerMode,
  ESaveGameLoadStatus, ERegionLoadStatus, EPlayerSpawnStatus, ESaveType; members retyped to their Ego_r.pdb types
  (HeroInformationScreenModeAfterTutorial, SaveGameLoadStatus, SaveGamePathName (CWideString), RegionLoadStatus,
  MostRecentSaveType / ...BeforeManualSave (ESaveType), MostRecentManualSaveName (CWideString), WeatherMaskingPrimitiveHandles
  (vector<CEnginePrimitiveHandle>), PlayerSpawnStatus); methods ResetToFrontEnd, OpenHeroInformationScreen, GetBulletTimeManager
  (inline), GetPathNameForFileName, LoadGameState, AutoLoadRespawn, SetAsPaused (inline), UpdateFrame, UpdateSaveGameLoad,
  UpdateRegionLoad, PrepareAtmosBanksForLoadedRegion, UpdateEnvironmentCameraPos, IsHeroStateFileValid, LoadGameStateInternal,
  LoadHeroStateInternal, AutoLoadInternal; static Frame 0x013B89BC (descriptive). The two offsetof asserts on the renamed
  members are gated by the branch.
- engine/CPlayer.h (FABLE_RETAIL_PLAYER): CPlayer : CInitBaseClass (RTTI), Update / UpdateFreeCamera / GetJoystickDeviceNumber,
  inline GetPlayerInterface. engine/CInitBaseClass.h: IsValid. engine/CGamePlayerInterface.h: new FABLE_RETAIL_CGAMEPLAYERINTERFACE
  branch (IsGameActionEventInQueue, forward enums EGameAction / EInputButtonEventType).
- engine/CPlayerGui.h (Update, UpdateMiniMapMarkers), CRegion.h (IsSoundThemesEnabled), CEnvironment.h (new branch:
  UpdateInterpolation, Update, IsEnvironmentRequiringWeatherMasks), CNavigatorManager.h (FrameUpdate), CBulletTimeManager.h
  (IsBulletTimeActive, IsTimeForFullUpdate, FrameUpdate), CThingManager.h (WorldUpdateThings, UpdateDeaths,
  IsNonScriptedEntitesPauseMode, PreparePrimitivesForEngine), CQuestManager.h (Update, IsBetweenQuestStartAndFinishScreens),
  CCombatManager.h (Update), CDriveManager.h (DeleteFileW), fable_game.h (CMainGameComponent::GetDisplayEngine inline),
  CTCBase.h (UpdateGlobalTCs), CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT: WorldUpdate), CWorldMap.h (const GetRegion,
  GetLoadedRegion).
- Maths (row 0x004A1CB0): C3DVector.h (unary operator-, operator^), CRightHandedSet.h (retail class under #else: GetUp,
  GetRight = Forward ^ Up), CMatrix3x4.h (__forceinline Set x2, PreScale, SetInverse), C3DBoundingSphere.h (ctor from centre
  and radius), CIVCountedPointer.h (counted copy ctor), CPhysicsMeshObject.h (Mesh retyped CIVCountedPointer<const
  CPhysicsMesh>, inline GetMesh). New engine/CEnginePrimitiveWeatherMask.h (ctor 0x0049D000; sizeof 0x6C).
- Quests (row 0x004A0D90): CWorld.h retail branch types InitialActiveQuests (CArray<CCharString>), RegisteredQuests
  (std::vector<CCharString>), ActiveTestQuests (std::vector<CTestQuest>) and adds LoadQuestsFromFile / ClearRegisteredQuests;
  new engine/CTestQuest.h (Ego_r.pdb layout, implicit members); CQuestManager.h RegisterQuest; fable_string.h
  (FABLE_RETAIL_CCHARSTRING) EqualsCaseInsensitive with its inline body.
- Save/load (row 0x004A5E90): CAFile.h (CFileScanInfo, StartFileScan), fable_user_profile.h (GetAutoSave*FileName,
  GetAutoSaveCheckPointPathName, GetPathNameFromFileName), fable_resource_directories.h (new FABLE_RETAIL_NRESOURCEDIRECTORYNAMES:
  GetDebugSaveDir).

## Siblings
sib/changed.txt lists the 1,551 landed sources whose preprocessed text changes under the final mirror (cpp on both sides with
the same system-header stubs). The ones that change only through FABLE_RETAIL_NSCRIPT (CGameScriptInterface::WorldUpdate
declaration), FABLE_RETAIL_CTCBASE (UpdateGlobalTCs declaration) or FABLE_RETAIL_CCHARSTRING (EqualsCaseInsensitive's inline
body; no landed row calls it) cannot change code; SIBLINGS.tsv regrades every other one plus every 40th of those (78 rows) with
vland --recheck-landed on the final mirror: 77 match + PASS, and 0x0047A9F0 CHeroLogBook::AddEntry is DIFFER(1109v1105) PASS
with or without the overlay (pre-existing). A first round on the earlier mirror (sib/SIBLINGS_round1_previous_mirror.tsv,
133 rows) found 10 behaviour-test FAILs (0x004DA967 .. 0x007F0890); all ten fail identically without the overlay.
Overlay rebase (after landing round d): every overlay header still contains every line of its current repo copy (diff shows
only this chunk's additions), so the round-d changes (CPlayer, C3DVector, fable_game.h ...) are in; the mirror is rebuilt from
the current rebuild/include on every grade.
