# Hand chunk 705 (NCameraModes::CScriptedControl .. Render2DDrawList): 14 of 14 byte-exact

Ten never-authored NA048 rows + four REPAIR rows. Every row grades MATCH / RELOCATION_MATCH in q.py (gall.sh) AND in the
read-only vland check (`--recheck-landed`, 14 WINS, behaviour PASS); tests PASS under g++ and the lane VC7.1 cl.exe.
Call targets checked with calls.py (every reloc goes to retail's address). No retail_asm, no volatile, no new pragma pack.

## What matched (14)

- **NCameraModes::CScriptedControl::Update** 0x0087BBC0 (MATCH): `PScriptModule->Update()` through engine/CCountedPointer.h
  operator->; the module's slot 1 is _purecall in vtable 0x01255F74 (pure Update). Name: vtable 0x0125C58C slot 1; Ego_r +
  FableWin `?Update@CScriptedControl@NCameraModes@@UAEXXZ`; Ego_r donor order GetDefaultFOV / GetDefaultRollAngle / OnInit /
  Update / IsShortenOnOcclusion / IsStretchAroundHero / OnDie = retail 0x0087BA30 .. 0x0087BBF0 exactly (slots 18/19/21/1/6/8/22).
- **CCreatureAction_LeapLong::FinishAction** 0x008B74A0 and **CCreatureAction_WhisperSommersaultForward::FinishAction**
  0x008C0B10 (byte twins, each named from its own vtable: 0x0127A9EC slot 4 / 0x01296754 slot 4; both PDBs spell both,
  public `UAE` over the protected base `MAE`). `if (PCreature.GetPItem()->HasTC(77)) PCreature.GetPItem()->RemoveTC(77);
  CCombatActionBase::FinishAction();` -- `test ch,20h` on +0x28 is the inline HasTC (bitset +0x20, dword 2, bit 13).
  77 = CTCSolidToThing (the only CTC GetInterfaceType returning 0x4D: vtable 0x0126A62C slot 21, 0x00770FF0).
  Model on engine/CBaseObject.h (retail opt-in) + engine/CClass.h layout-rename (CThing : CClass : CBaseObject).
- **NCreatureMode::CModeUnderground::OnDeactivate** 0x008D6710: `if (ThingTypeFlags & 0x10) ((CThingAICreature*)thing)->
  SetAsImmuneToAttack(false); CModeStandBase::OnDeactivate(); RevertOldTargetFlags(*thing, OldTargetFlags);` -- the mirror
  of the landed OnActivate 0x008D66E0; FableWin emits the inline `?SetAsImmuneToAttack@CThingAICreature@@QAEX_N@Z` right after
  OnActivate (ImmuneToAttack = bit 6 of +0x1C8, landed SetHasInformation). Slot 6, both PDBs.
- **NCreatureMode::CModeDragonFireBeam::OnDeactivate** 0x008DBB00: KillBeamEffect(); `if (Type == 1) SuspendAI(false, this)`;
  CModeStandBase::OnDeactivate(). The GuildmasterPowerUpHeal 0x008D69A0 shape plus the private KillBeamEffect call.
- **CAIStateGroup_MinionSitOnSeatBase::Update** 0x0090F740 (protected `MAE`): `if (MoveToSeatAndSitDown(3)) while
  (AIAction_DoNothing() == 4 && IsStateGroupStillValid()); Terminate();`. Slot 12 of 0x0128B584; the other five references
  are derived vtables inheriting the slot (MinionSitAt*/SitOnSeat), so one name. IsStateGroupStillValid = slot 14 (+0x38).
- **CAIStateGroup_BirdFlyToFood::Update** 0x00913F00: `FrameLastFoundFood = PeekWorld().GetFrame(); AIAction_RandomFlyToPos(
  FoodPos); Terminate();` -- the static GetFrame reached through the PeekWorld() object expression keeps the PeekWorld call
  (result dropped). Ego_r layout FoodPos +0x24, FrameLastFoundFood +0x30.
- **CSoundManagerLH::StopAllSounds** 0x00A3EBC0 (MATCH, DESCRIPTIVE name): STLport list iteration over the emitter list +0x4C
  calling each emitter's slot 10, then m_pMain (+0x44). The catalog CNavigatorManager label is propagated: the only reference
  is slot 16 of the CSoundManagerLH vtable 0x0129D2C4. The sound module is in neither PDB, so no proven name exists.
  Extent checked: int3 from 0x00A3EBE9 to 0x00A3EBF0.
- **CNavQuadTreeNode::RemoveNeighbours** 0x00A75E00 (MATCH, protected `MAE`): TL/TR/BL/BR->RemoveNeighbours() (slot 19
  recursion). Ego_r donor order CalculateChildrenNeighbours / RemoveNeighbours / SetUpNeighbours = 0x00A75DA0 / 0x00A75E00 /
  0x00A75E30. Extent checked (int3 0x00A75E24..0x00A75E2F). The base slot is protected: `friend class CNavQuadTreeNode` in the
  CANavQuadTreeNode model (retail code calls the protected virtual through CANavQuadTreeNode* children, which C++ only allows
  to a friend).
- **CEngine::GetEditorAnimation** 0x00B23CB0: `index = g_pResourceManager->SetupEditorAnimation(name); const CMeshDataBank*
  bank = g_pResourceManager->MeshBanks[1]; return bank->PeekAnimation(index);` -- SHAPE: the named const bank local puts the
  argument-slot store after the two ecx loads (inline object expression: store first; 2 probes).
- REPAIRS: see below (0x00692230, 0x006922C0, 0x004D9861, 0x009E1440).

## Repairs (cause and fix)

- **0x00692230 / 0x006922C0** NCreatureAction::CActionEventExplosion / CActionEventRemoveObject default ctors.
  Cause: NOT an EH frame. The base flags have no /EHsc and the object holds no __ehhandler; the landed source still compiles
  to the retail 30 bytes (q.py and `vland --recheck-landed` with leaf `??0CActionEventExplosion@NCreatureAction@@QAE@XZ`:
  RELOCATION_MATCH). Since 84edb11e4 put the class on engine/CBaseClass.h, its implicit virtual dtor makes the TU emit
  `??_GCActionEventExplosion` (40 bytes: dtor call, `test [esp+8],1`, delete, `ret 4`) through the vtable; the landed source had
  NO `// Leaf:` line, so the verifier graded the largest symbol containing the class name -- the ??_G -- hence DIFFER(40v30).
  I reproduced that exact DIFFER(40v30) by dropping the leaf, and confirmed the 40 bytes are the ??_G with objdump.
  Fix: `// Leaf: ??0...` plus the real model: Ego_r CActionEventBase {Finished +4, ActivationFrame +8, PThing +0xC,
  PCameraManager +0x10} (abstract, `__declspec(novtable)`: the Finished store precedes the derived vtable store, the base's
  own store is dead), CActionEventExplosion {AttachmentObject +0x14, ExplosionDefIndex, Dummy, UseActionHelper},
  CActionEventRemoveObject {Hand +0x14}. Retail had the same virtual-dtor chain (Ego_r ??_GCActionEventRemoveObject exists),
  so the shared CBaseClass.h is right; only the leaf was missing.
- **0x004D9861** -> `CFireheartMinigameDef::~CFireheartMinigameDef` (implicit, `??1CFireheartMinigameDef@@UAE@XZ`).
  Cause: the landed source invented `__declspec(novtable) struct CDefPointeeBase_Derived_004d9861 : CDefPointeeBase` with a
  user dtor; its line-1 name `~CDefPointeeBase_Derived_004d9861` matches no symbol (q.py's default leaf keeps the `~`), so the
  largest TU function was graded (36v33). Identity: the only caller is ??_G 0x004D9845 = slot 0 of vtable 0x0123E584 (RTTI
  CFireheartMinigameDef : CSubDefClassBase : CDefClassBase : CDefPointeeBase : CResource); the ctor 0x004D6638 stores that
  vtable; GetSizeofClass 0x004D665A returns 0x3C; Ego_r + FableWin `??1CFireheartMinigameDef@@UAE@XZ`; Ego_r layout MoonFX
  +0x28 / SunFX +0x2C / QuitText / YesText / NoText. Fix: the real class on the shared def headers (FABLE_RETAIL_DEF_CLASS_BASE,
  engine/CSubDefClassBase.h); the compiler-generated dtor (dead base vtable stores, only ~CDefPointeeBase's 0x01230BA0 kept);
  retail's own out-of-line ctor emits the vtable (no emission helper needed). `#pragma optimize("s", on)`: the region has no
  padding and with the pragma the same TU also reproduces retail's ??_G 0x004D9845 (`pop ecx`) and ctor 0x004D6638 byte for
  byte -- that is how retail built it (the dtor bytes match either way).
- **0x009E1440** -> `std::vector<CTVertexRHWColSpecTex1>::erase(T*, T*)` (STLport). Cause of TCC_FAIL: the July-era test
  `#include`d the landed source by a relative path plus fable_render2d_vertex_queue.h and printed
  `FABLETLC_RENDER2D_COPY_BLOCK_BEHAVIOR PASS`, not `009e1440_TEST PASS`. Identity: callers 0x009DB678 (`erase(begin(), end())`
  on CRenderManager2D +0x3E88 = Ego_r QuickDraw2DVertices, vector<CTVertexRHWColSpecTex1>, class key V, 32 bytes) and the
  out-of-line clear() 0x009E14A0. Fix: STLport 4.6 erase (`__copy_ptrs` -> inline random-access `__copy`, `_Destroy`,
  `_M_finish = __i; return __first;`) with an explicit instantiation; standalone test.

## Parked for Fable 5.1 (0)

## Skipped

None. No row holds two functions; both "no ret/jmp" extents (0x00A3EBC0, 0x00A75E00) end in a sibling `jmp [reg+N]` followed
by int3 padding up to the next function.

## Label corrections

- 0x0087BBC0 vfunc_1 -> NCameraModes::CScriptedControl::Update.
- 0x00A3EBC0 CNavigatorManager::InterruptAllNavigators -> CSoundManagerLH::StopAllSounds (descriptive; wrong class).
- 0x004D9861 _unattributed::~CDefPointeeBase_Derived_004d9861 -> CFireheartMinigameDef::~CFireheartMinigameDef.
- 0x009E1440 Render2DDrawList::CopyBlock -> std::vector<CTVertexRHWColSpecTex1>::erase.
- Callees: 0x004C9840 CBaseObject::OnPropertyChanged -> CThing::RemoveTC; 0x009130D0 ExecuteFlyingPath -> AIAction_RandomFlyToPos;
  0x00B3BC90 CEngine::GetEditorAnimation -> CEngineResourceManager::SetupEditorAnimation; 0x00A242C0 GetAbilityLevel ->
  CMeshDataBank::PeekAnimation; 0x008FCF40 CXMVPlaybackInfo -> CAIStateGroupBase::Terminate; 0x008FCFA0 CPlayer::GetCurrentMode
  -> CAIStateGroupBase::PeekWorld; 0x008DBA90 CastWillReclaimUndeadFollower::PerformAction -> CModeDragonFireBeam::KillBeamEffect.
  All in NAME_FIXES.tsv (18 rows) with the wrong bound spellings.
- The other ten rows' catalog labels were right in substance (the forge `_global::` stand-ins aside).

## Names and evidence (callees / globals)

- Proven (two sources each): every row's own symbol except 0x00A3EBC0; callees RemoveTC, HasTC (inline), GetPItem, FinishAction
  (CCombatActionBase, MAE), OnDeactivate@CModeStandBase, RevertOldTargetFlags (__fastcall), SuspendAI, KillBeamEffect,
  MoveToSeatAndSitDown, AIAction_DoNothing, Terminate, PeekWorld, CWorld::GetFrame (ESTABLISHED), AIAction_RandomFlyToPos,
  SetupEditorAnimation, PeekAnimation, CCharString ctor/dtor (fable_string.h), CBaseClass (engine/CBaseClass.h).
- Descriptive: CSoundManagerLH::StopAllSounds (row), CSoundEmitterLH::Shutdown (emitter slot 10, the catalog name kept, spelled
  virtual), CThing::PresentTCs (bitset member), CModeUnderground::OldTargetFlags (+0x74).
- Global 0x01436E98: `?g_pResourceManager@@3PAVCEngineResourceManager@@A`, the landed spelling the c600/c672 queue rows name; ~20
  spellings remain bound and no PDB data symbol exists (coordinator decision still open).

## Header debt

- engine/CClass.h, CNavQuadTreeNode.h, CANavQuadTreeNode.h, CEngine.h, CEngineResourceManager.h, CMeshDataBank.h, CWorld.h,
  CAIStateGroupBase.h, CModeStandBase.h, CCreatureAction_LeapLong.h: generated flattened layouts (vptr as a data member, no
  member functions), so a polymorphic derived class or a member call cannot be modelled on them. CClass is used through the
  layout-rename include; the others are local models with the header's offsets (vland WARN SHARED_HEADER_AVAILABLE for
  CWorld / CEngine / CEngineResourceManager / CMeshDataBank on 0x00913F00 / 0x00B23CB0).
- No retail-opt-in header exists for CThing / CThingAICreature / CTCCreatureModeManager::CModeBase / CASoundManager /
  CSoundManagerLH / CScriptModuleBase: local models with the landed rows' offsets.

## Decisions for the coordinator

1. CSoundManagerLH's list at +0x4C: the landed spelling is `std::list<CSoundObjectLH*>` (descriptive class, ~_List_base
   0x00A3FF60 bound under it), but the elements are CSoundEmitterLH objects (slot 4 0x00A3ECB0 news a CSoundEmitterLH and
   push_backs it; this row calls emitter slot 10 on each). I modelled `std::list<CSoundEmitterLH*>`; no symbol of this row
   depends on it. Respelling the landed _List_base would change its leaf -- your call.
2. 0x00A3EBC0's name is descriptive (StopAllSounds). Its vtable siblings 0x00A3EB20 / 0x00A3EBF0 / 0x00A3EC30 (slots 3 / 14 /
   15) are landed under address-suffixed stand-in classes (CListOwner_00a3eb20, CEngineManager_00a3ebf0, CHandlerList_00a3ec30)
   that are really CSoundManagerLH members; worth a family respell once a naming source for the sound module exists.
