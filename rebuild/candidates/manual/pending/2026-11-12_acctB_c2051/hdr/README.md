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
