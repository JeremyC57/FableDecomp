# Chunk 2051 header overlay (all changes opt-in; default text of every repo header unchanged)

New headers (no repo copy existed; no landed includers):
- engine/CGameNavigatorManagerExternalInterface.h -- CNavigatorManagerExternalInterface root + CGameNavigatorManagerExternalInterface
  (vtable 0x01238C58, World +4; inline ctor and empty inline virtual dtors: CWorld::Init builds a stack temporary with no dtor call).
- engine/CScriptConversationManager.h -- Ego_r.pdb members (map<long,CScriptConversation> +0, NextConversationIndex +0xC,
  AlwaysShowText +0x10), no user dtor (checked_delete 0x004AE810 expands the map dtor).
- engine/CSkeletalMorphResourceManager.h -- CTBaseSingleton<CSkeletalMorphResourceManager>, CachedMorphs map +4, sizeof 0x10.
- engine/CStopWatch.h, CXMVPlayerBuffers.h, CEngineInterpolationInfo.h, CMainGameComponentInit.h, CFrontendGameComponent.h,
  CFrontEndDef.h (retail: CParentDefClassBase + vAttractModeMovie +0x3C), CMovieInfo.h, CXMVPlayer.h (statics descriptive) --
  Ego_r.pdb layouts; used by the parked 0x0042EC7C.

Opt-in blocks in copies of repo headers (each documented at the block with its evidence):
- CWorld.h FABLE_RETAIL_CWORLD_TYPED_MEMBERS: +0x14..+0x7C typed as Ego_r.pdb boost::scoped_ptr<T> / CCountedPointer<T> under the
  PDB names (same offsets), protected InitThingManager / InitMeshBank declarations, the seven always-on accessors respelled, the
  flattened-name offset asserts swapped for typed ones.
- CWorldMap.h _CTOR, CEnvironment.h _CTOR (inside _VIRTUALS), CNavigatorManager.h _VIRTUALS (real vptr, ctor, ~ slot 0,
  AddNewClassType slot 1), CFactionManager.h _CTOR, CCombatManager.h _CTOR (ctor + Init), CMessageEventManager.h _CTOR,
  COpinionReactionManager.h _CTOR, CThingSearchTools.h _IMPLICIT_DTOR, CGameTimeManager.h _IMPLICIT_DTOR, CGameCameraManager.h _CTOR
  (+ Update / GetRenderCamera override declarations), CGameCameraBase.h _CTOR (+ Update), CBulletTimeManager.h _CTOR,
  NUISystem_CManager.h FABLE_RETAIL_CMANAGER_WORLDINIT (SetMeshBank, inline SetInputManager, SetInput), CCreatureActionBase.h
  _STATREGISTER, CMainGameComponent.h _ENGINEGRAPHICS (PMainGraphicBank typed + inline PeekEngineGraphics).
- For the parked Run: CNewFrontendGameComponent.h _RUN, CSystemManager.h _FORCEMUSICPLAY (inline Get/ResetForceMusicPlay, FITTED
  bodies), CFrontEndManager.h _RUN, CTBaseSingleton.h _RELEASE, CFontManager.h _ASYNC, CInputManager.h _CLEAREVENTS,
  fable_retail_banks.h _CLOSERETAILBANK, NUISystem_CUIScreenUnitConverter.h _RESETDIMENSIONS, CRGBColour.h
  _FORCEINLINE_COPY (same copy body, FABLE_FORCEINLINE; size TU refuses the plain inline).
No shared inline body text was changed on a default path, so no landed row needs a regrade.

# Chunk 2052 additions (all opt-in; default text of every repo header unchanged, so no landed includer needs a regrade)
Copied from agent2051 (read-only source) as the base overlay; then added:
- engine/CPersistContextWriteSection.h (NEW): Ego_r.pdb CPersistContextWriteSection (Name CCharString +0, StartPosition +4, sizeof 8) with
  the inline ctor `: StartPosition(0) {}` / empty dtor (??0 / ??1 in Ego_r.pdb; expanded at 0x0049F532 / 0x0049FF5C).
- CWorld.h: FABLE_RETAIL_CWORLD_SAVEGAMESTATEINTERNAL (SaveGameStateInternal, Start/StopAutoSaveProgress: protected IAE in Ego_r + FableWin),
  FABLE_RETAIL_CWORLD_LOADGAMESTATEINTERNAL (PrepareForLoad, Load(const CWideString&), ActivateWorld(bool): Ego_r QAE), FABLE_RETAIL_CWORLD_TYPED_SAVE
  (CurrentWorldName as CCharString +0x84, SaveGameMarkerPos as C3DVector at +0x141 -- needs FABLE_RETAIL_C3DVECTOR_PACKED, retail passes
  `lea [esi+141h]` -- and GuildSealRecallPos as C3DVector +0x154; Ego_r.pdb member types; the offset asserts switch with it).
- CPersistContext.h FABLE_RETAIL_CPERSISTCONTEXT_WRITESECTIONS: Init(CDataOutputStream&, bool) 0x009BAA50, StartSection 0x009BAB60, EndSection
  0x009BABF0 (Ego_r.pdb spellings; declarations only).
- CDataOutputStream.h _ZLIB (WriteZlibCompressed 0x009967E0, WriteRepeatedByte 0x00994230), CMemoryDataOutputStream.h _RESERVE (ReserveSize
  0x00996BC0, Clear 0x00996B60), CAFile.h _ISROOMFORFILE (static __fastcall IsRoomForFile 0x00999AB0), CThingManager.h _SAVEGAMESTATE /
  _LOADGAMESTATE, CQuestManager.h _SAVEGAMESTATE / _LOADGAMESTATE_CONTEXT (+ DeactivateAllQuests), CWorldMap.h _SAVEGAMESTATE / _LOADGAMESTATE,
  CPlayerManager.h _SAVEGAMESTATE / _LOADGAMESTATE, CFactionManager.h _SAVEGAMESTATE (const) / _LOADGAMESTATE, NProgressDisplay.h _STARTPROGRESS
  (__fastcall six bools 0x009E9EF0), fable_resource_directories.h _LEVELDIR (GetLevelDir 0x0041A060): Ego_r.pdb declarations only.
- CTCInGameMenu.h _PAUSEDTIME: FableWin.pdb inline Get/SetAccumulativePausedTime (+0x70 read / written in place by 0x0049F9BC / 0x004A2C79;
  needs the _SAVELOAD layout).
- CStringParser.h _INIT_STRING_INLINE: the body of Ego_r.pdb ?Init@CStringParser@@QAEXABVCCharString@@P6IX0@Z0J@Z, inline in retail
  (TempCharString = string, then the const char* Init on the argument's characters, 0x004A2624..0x004A2653).
Rows graded with this overlay: 0x0049F4C0, 0x004A21F0 (both RELOCATION_MATCH after the last header change, grades 38 / 39).
