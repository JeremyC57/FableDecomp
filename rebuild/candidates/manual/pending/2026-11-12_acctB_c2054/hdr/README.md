# Chunk 2054 header overlay (row 0x00A7AF60 CNavQuadTree::LoadFromFile; all changes opt-in, default text of every repo header unchanged)

Files this chunk added or changed (five; everything else in this directory is chunk 2051's overlay carried over unchanged, see below):
- engine/CCachedFile.h -- NEW (no repo copy). The polymorphic retail class: RTTI .?AVCCachedFile@@ : CAFile : CBaseClassNonCopyable,
  vtable 0x012724D4, sizeof 0x28; members under their Ego_r.pdb names at retail offsets (Position +4, BufferStartPosition +8,
  DataBuffer +0xC, Pathname +0x18, PDiskFile +0x1C, Opened +0x24, BufferValid +0x25: the landed ctor 0x00A5B590 and ~CCachedFile
  0x00826380 models); methods in the Ego_r.pdb / FableWin.pdb spellings. The destructor is INLINE (`virtual ~CCachedFile() { Close(); }`):
  the row calls its out-of-line copy 0x00826380 on the version exit and expands it at the final exit, and the row TU's own
  ??1CCachedFile@@UAE@XZ equals retail 0x00826380 instruction for instruction (95 bytes, tmp/sibs_final.txt). Defines
  FABLE_RETAIL_CAFILE for its base. No landed includer.
- engine/CANavQuadTreeNode.h -- nested opt-in FABLE_RETAIL_CANAVQUADTREENODE_LOADSLOTS (inside FABLE_RETAIL_CANAVQUADTREENODE): the
  protected virtual slots 16..22 in PDB order under the Ego_r.pdb spellings of the overrides (SetUpNeighboursWithLeaf, SetUpNeighbours,
  CalculateChildrenNeighbours, RemoveNeighbours, AddToIndexTable, SaveToFile, ConvertIndexesToPointers) plus the empty enum
  ENavQuadTreeNeighbours and `#include "stlport/fable_map.h"`. Evidence: Ego_r.pdb ?ConvertIndexesToPointers@CNavQuadTreeNode@@MAEX
  ABV?$map@JPAVCANavQuadTreeNode@@..@Z / @CNavNavigableLeafNode@@ (two overrides) and the row's `call [eax+58h]` at 0x00A7B799
  (slot 22). Default text unchanged.
- engine/CNavQuadTreeNode.h -- opt-in FABLE_RETAIL_CNAVQUADTREENODE: the retail class (: CANavQuadTreeNode, vtable 0x0129DCD4, sizeof
  0x20 = `push 20h ; call operator new` at 0x00A7B68D; Ego_r.pdb TLNode +0x10 .. BRNode +0x1C) with the file ctor
  ??0CNavQuadTreeNode@@QAE@ABVC2DVector@@EEAAVCAFile@@@Z 0x00A76580 (Ego_r.pdb + FableWin.pdb, landed). The flattened default stays.
- engine/CNavNavigableLeafNode.h -- opt-in FABLE_RETAIL_CNAVNAVIGABLELEAFNODE: the retail class (vtable 0x0129DDA4, sizeof 0x20 =
  `push 20h` at 0x00A7B653; Ego_r.pdb PRegion +0x10, Neighbours +0x14 (CSmallVector<const CANavQuadTreeNode*, 8>: Start +0, Size +4),
  PreferabilityCost +0x1C) with the file ctor ??0CNavNavigableLeafNode@@QAE@ABVC2DVector@@EEAAVCAFile@@ABV?$list@VCNavRegion@@..@std@@_N@Z
  0x00A772D0 (Ego_r.pdb + FableWin.pdb). The flattened default stays.
- engine/CNavSwitchableLeafNode.h -- opt-in FABLE_RETAIL_CNAVSWITCHABLELEAFNODE: the retail class (: CNavNavigableLeafNode, vtable
  0x0129DE04, sizeof 0x30 = `push 30h` at 0x00A7B55F; Ego_r.pdb UniqueIDsOpen +0x20, CVectorMap<unsigned __int64, bool>) with the file
  ctor 0x00A79A30 and ?SetAsBlocked@CNavSwitchableLeafNode@@QAEX_K_N@Z 0x00A78800 (both Ego_r.pdb + FableWin.pdb). Default stays.

Not in the overlay any more (worker, 2026-11-12):
- stlport/fable_containers.h: the interrupted session's FABLE_RETAIL_STLP_COPY_NOINLINE (`__declspec(noinline)` __copy) and
  FABLE_RETAIL_STLP_LIST_ALLOC_CREATE opt-ins are WITHDRAWN. The row needs neither: with the repo's existing
  FABLE_RETAIL_STLP_VECTOR_ASSIGN the TU's __copy<CNavigationLayer*, ..> has retail's body (0x00A85870) and VC7.1 calls it by itself.
  The row compiles against the unmodified repo stlport headers. (Old overlay text kept in tmp/fable_containers.copy_noinline_overlay.h.)
- engine/CAFile.h, CNavQuadTree.h, CVectorMap.h: the interrupted session's copies were byte-identical to the repo files; removed
  (kept in tmp/hdr_same/), the row regraded RELOCATION_MATCH without them (grades.log 33).

Repo opt-ins the row uses unchanged: FABLE_RETAIL_C2DVECTOR, _C2DBOXF, _C2DBOXI, _C2DCOORDI_TOFLOAT, _CANAVQUADTREENODE, _CNAVQUADTREE,
_CNAVBLOCKEDQUADTREENODE, _CNAVIGATIONPOSITION_CTORS, _SUSPENDABLE_PROCESS, _CAFILE, _STLP_VECTOR, _STLP_VECTOR_ASSIGN, _STLP_MAP,
_STLP_SORT_FASTCALL, _CVECTORMAP_CTOR / _INSERT_KEYVALUE / _INSERT_VALUE_TYPE / _SORT.
No shared inline body text was changed on a default path (all five changes are new files or `#ifdef` blocks), so no landed row needs
a regrade. Free model check of the row TU against retail: tmp/sibs_final.txt (39 other functions equal their retail copies).

The rest of this file is chunk 2051's README; its headers sit in this directory untouched and are not used by the 2054 row.

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

Continuation agent (2026-11-12): no header text changed. The matched Run uses FABLE_RETAIL_CRGBCOLOUR_FORCEINLINE_COPY with its
`// Inline-budget:` line (c2 inline trace: copy ctor size 31, savings 5 -> 26 > size-TU limit 15, refused with no score). CWorld::Init
regraded RELOCATION_MATCH under the same overlay.
